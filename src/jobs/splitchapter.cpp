// Copyright (C) 2020 Vincent Chambrin
// This file is part of the 'ffmpeg-gui' project
// For conditions of distribution and use, see copyright notice in LICENSE

#include "jobs/splitchapter.h"

#include "ffmpeg.h"

#include "media.h"
#include "media/stream.h"

#include "pythonscript.h"

#include <QFileInfo>
#include <QProcess>

#include <QDebug>

const std::string SplitChapter::ClassName = "SplitChapter";

SplitChapter::SplitChapter(int num, std::shared_ptr<Media> media, std::vector<size_t> which_chapters, std::vector<std::shared_ptr<Stream>> which_streams)
  : Job(num),
  m_input(media), 
  m_chapters(std::move(which_chapters)),
  m_streams(std::move(which_streams))
{
  setDescription("Split a video file based on its chapters.");
}

SplitChapter::~SplitChapter()
{

}

const std::string& SplitChapter::kind() const
{
  return ClassName;
}

void SplitChapter::run()
{
  if (state() == CANCELLED)
  {
    m_current_chapter = 0;
  }

  setState(RUNNING);

  processNextChapter();
}

void SplitChapter::cancel()
{
  if (m_ffmpeg)
  {
    if (m_ffmpeg->isRunning())
    {
      m_ffmpeg->kill();
    }

    m_ffmpeg = nullptr;
  }

  setState(CANCELLED);
}

void SplitChapter::exportAsPython(const std::string& folder) const
{
  PythonScript script{ *this, folder };

  script.importOsPath();
  script.importSubprocess();

  script.newline();

  {
    script.def(script.fun(*this));

    for (size_t i(0); i < m_chapters.size(); ++i)
    {
      std::vector<std::string> args = computeArgs(m_chapters.at(i));

      script.If(script.Not(script.osPathExists(script.str(args.back()))));

      script.CallFFmpeg(args);

      script.EndIf();
    }

    script.enddef();
  }

  script.newline();

  script.callIfMain(script.fun(*this));
}

std::vector<std::string> SplitChapter::computeArgs(size_t chap_index) const
{
  const Chapter& chap = m_input->chapters().at(chap_index);

  const std::string start = std::to_string(chap.start());
  const std::string end = std::to_string(chap.end());

  QFileInfo file{ QString::fromStdString(m_input->name()) };
  const std::string output_file = (file.baseName() + "-chap-" + QString::number(chap.num()) + "." + file.suffix()).toStdString();

  std::vector<std::string> args;
  args.push_back("-i");
  args.push_back(m_input->name());
  args.push_back("-map_chapters");
  args.push_back("-1");

  if (!m_streams.empty())
  {
    for (const std::shared_ptr<Stream>& s : m_streams)
    {
      args.push_back("-map");
      args.push_back("0:" + std::to_string(s->num()));
    }
  }

  args.push_back("-vcodec");
  args.push_back("copy");
  args.push_back("-acodec");
  args.push_back("copy");
  args.push_back("-ss");
  args.push_back(start);
  args.push_back("-to");
  args.push_back(end);
  args.push_back(output_file);

  return args;
}

void SplitChapter::processNextChapter()
{
  if (state() == CANCELLED)
  {
    return;
  }

  if (m_current_chapter == m_chapters.size())
  {
    setState(DONE);
    return;
  }

  std::vector<std::string> args = computeArgs(m_chapters.at(m_current_chapter));
  remove(args.back());

  m_ffmpeg = std::make_unique<FFMPEG>(args);
}

void SplitChapter::update()
{
  if (m_ffmpeg->isFinished())
  {
    m_current_chapter++;
    setProgress((static_cast<int>(m_current_chapter) - 1) * 100 / static_cast<int>(m_chapters.size()));
    m_ffmpeg = nullptr;
    processNextChapter();
  }
}
