// Copyright (C) 2020 Vincent Chambrin
// This file is part of the 'ffmpeg-gui' project
// For conditions of distribution and use, see copyright notice in LICENSE

#include "ffmpeg.h"

#include "media.h"
#include "media/video.h"
#include "media/crop.h"

#include "parser.h"

#include <QDir>
#include <QFile>
#include <QPixmap>
#include <QProcess>
#include <QRegularExpression>

#include <QDebug>

#include <cassert>
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
  if (m_process->state() == QProcess::Running)
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
    while (m_process->canReadLine())
    {
      std::string line = m_process->readLine().constData();
      if (line.back() == '\r')
        line.pop_back();

      double time = 0.;
      const bool parse_progress = Parser::parseProgressTime(line, time);

      if (m_input_duration > 0. && parse_progress)
        m_progress = time / m_input_duration;

      if (!parse_progress)
        m_output += line;
    }

    {
      std::string text = m_process->readAll().constData();

      if (text.empty())
        return;

      double time = 0.;
      const bool parse_progress = Parser::parseProgressTime(text, time);

      if (m_input_duration > 0. && parse_progress)
        m_progress = time / m_input_duration;

      if (!parse_progress)
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

Crop FFMPEG::cropdetect(const Media& media, int nbsamples)
{
  assert(media.hasVideo());

  const Crop unknown_crop{ 0, 0, std::numeric_limits<int>::max(), std::numeric_limits<int>::max() };
  Crop result = unknown_crop;

  const double interval = media.duration() / (nbsamples + 1);

  for (int i(0); i < nbsamples; ++i)
  {
    const double t = (i + 1) * interval;

    std::vector<std::string> args;
    args.push_back("-ss");
    args.push_back(std::to_string(t));
    args.push_back("-i");
    args.push_back(media.name());
    args.push_back("-t");
    args.push_back("1");
    args.push_back("-vf");
    args.push_back("cropdetect");
    args.push_back("-f");
    args.push_back("null");
    args.push_back("-");

    FFMPEG ffmpeg{ args };
    ffmpeg.waitForFinished();

    size_t pos = ffmpeg.output().find("crop=");

    if (pos == std::string::npos)
    {
      qDebug() << "Could not parse crop";
      continue;
    }

    size_t crop_end = ffmpeg.output().find('\n', pos);

    try
    {
      Crop crop = Parser::parseCrop(ffmpeg.output().substr(pos + 5, crop_end - pos - 5));

      result.width = std::max(crop.width, result.width);
      result.height = std::max(crop.height, result.height);
      result.x = std::min(crop.x, result.x);
      result.y = std::min(crop.y, result.y);
    }
    catch (const std::runtime_error&)
    {

    }
  }

  if(result == unknown_crop)
    throw std::runtime_error{ "Could not detect crop" };

  return result;
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
