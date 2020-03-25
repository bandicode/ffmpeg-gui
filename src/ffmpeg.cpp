// Copyright (C) 2020 Vincent Chambrin
// This file is part of the 'ffmpeg-gui' project
// For conditions of distribution and use, see copyright notice in LICENSE

#include "ffmpeg.h"

#include "media.h"
#include "media/video.h"

#include "parser.h"

#include <QDir>
#include <QFile>
#include <QPixmap>
#include <QProcess>
#include <QRegularExpression>

#include <QDebug>

#include <stdexcept>

FFMPEG::FFMPEG()
  : m_process(nullptr)
{

}

FFMPEG::FFMPEG(const std::vector<std::string>& args)
  : m_process(new QProcess)
{
  m_process->setProgram("ffmpeg");

  QStringList qt_args;

  for (const std::string& str : args)
    qt_args << QString::fromStdString(str);

  m_process->setArguments(qt_args);
 
  m_process->setReadChannel(QProcess::StandardError);
  QObject::connect(m_process, &QProcess::readyReadStandardError, [this]() {update(); });

  m_process->start();

  if (!m_process->waitForStarted())
    throw std::runtime_error{ "Failed to start FFMPEG" };
}

FFMPEG::~FFMPEG()
{
  if (m_process)
  {
    if (m_process->state() != QProcess::NotRunning)
      m_process->kill();

    m_process->deleteLater();
  }
}

bool FFMPEG::isRunning() const
{
  return m_process && m_process->state() == QProcess::Running;
}

bool FFMPEG::isFinished() const
{
  return m_process && m_process->state() == QProcess::NotRunning;
}

void FFMPEG::kill()
{
  if (m_process && m_process->state() != QProcess::NotRunning)
  {
    m_process->kill();
  }
}

void FFMPEG::waitForFinished()
{
  if(m_process->state() == QProcess::Running)
    m_process->waitForFinished(-1);
}

FFMPEG::ExitStatus FFMPEG::exitStatus() const
{
  return static_cast<ExitStatus>(m_process->exitStatus());
}

int FFMPEG::exitCode() const
{
  return m_process->exitCode();
}

inline static bool starts_with_output(const std::string& str)
{
  return str.size() >= 6 && std::memcmp(str.data(), "Output", 6) == 0;
}

inline static bool starts_with_video(const std::string& str)
{
  return str.size() >= 6 && std::memcmp(str.data(), "video:", 6) == 0;
}

void FFMPEG::update()
{
  if (!m_enconding_started)
  {
    while (m_process->canReadLine())
    {
      std::string line = m_process->readLine().constData();
      if (line.back() == '\r')
        line.pop_back();

      m_output += line;

      if (!m_duration_parsed)
      {
        m_duration_parsed = Parser::parseInputDuration(line, m_input_duration);
      }
      else
      {
        if (starts_with_output(line))
        {
          m_enconding_started = true;
        }
      }
    }
  }
  else
  {
    std::string text = m_process->readAll().constData();

    double time = 0.;
    
    if (m_input_duration > 0. && Parser::parseProgressTime(text, time))
    {
      m_progress = time / m_input_duration;
    }

    size_t newline = text.rfind('\n', text.size() - 2);

    if (newline != std::string::npos)
    {
      text = std::string(text.begin() + newline + 1, text.end());

      if (text.size() > 2 && text.at(text.size() - 2) == '\r')
      {
        text.pop_back();
        text.pop_back();
        text.push_back('\n');
      }
    }

    if (starts_with_video(text))
    {
      m_output += text;
    }
  }
}

std::string FFMPEG::version()
{
  QProcess proc;
  proc.start("ffmpeg");

  if (!proc.waitForFinished())
    throw std::runtime_error{ "FFMPEG::version() : timeout" };

  QByteArray ba = proc.readAllStandardError();

  QRegularExpression regexp{ "ffmpeg version (\\d)\\.(\\d)\\.(\\d) Copyright" };
  QRegularExpressionMatch match = regexp.match(QString::fromUtf8(ba));

  if (!match.hasMatch()) 
    throw std::runtime_error{ "FFMPEG::version() : could not fetch version" };

  return (match.captured(1) + "." + match.captured(2) + "." + match.captured(3)).toStdString();
}

std::shared_ptr<Media> FFMPEG::info(const std::string& path)
{
  QProcess proc;
  proc.start("ffmpeg", QStringList{"-hide_banner", "-i", QString::fromStdString(path)});

  if (!proc.waitForFinished())
    throw std::runtime_error{ "FFMPEG::info() : timeout" };

  QByteArray ba = proc.readAllStandardError();

  Parser parser;
  return parser.parseMediaInfo(ba.toStdString());
}

QPixmap FFMPEG::snapshot(const Media& media, double time)
{
  std::string output_path = QDir::tempPath().toStdString() + "/ffmpeg-gui-snap.jpg";
  auto video = media.video();

  int realwidth = video->width() * video->sar().first / video->sar().second;
  int realheight = video->height();
  std::string size = std::to_string(realwidth) + "x" + std::to_string(realheight);

  std::vector<std::string> args;
  args.push_back("-ss");
  args.push_back(std::to_string(time));
  args.push_back("-i");
  args.push_back(media.name());
  args.push_back("-s");
  args.push_back(size);
  args.push_back("-vframes");
  args.push_back("1");
  args.push_back(output_path);

  FFMPEG ffmpeg{ args };
  ffmpeg.waitForFinished();

  QPixmap result{ QString::fromStdString(output_path) };
  QFile::remove(QString::fromStdString(output_path)); 
  return result;
}
