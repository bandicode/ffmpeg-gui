// Copyright (C) 2020 Vincent Chambrin
// This file is part of the 'ffmpeg-gui' project
// For conditions of distribution and use, see copyright notice in LICENSE

#include "jobs/convert.h"

#include "ffmpeg.h"

#include "media.h"
#include "media/stream.h"

#include "pythonscript.h"

#include <QFileInfo>

#include <QDebug>

const std::string Convert::ClassName = "Convert";

Convert::Convert(int num, std::shared_ptr<Media> in, std::vector<std::string> opts)
  : Job(num),
  m_input(in), 
  m_opts(std::move(opts))
{
  setDescription("Convert a file");
}

Convert::~Convert()
{

}

const std::string& Convert::kind() const
{
  return ClassName;
}

void Convert::run()
{
  setState(RUNNING);

  std::vector<std::string> args = computeArgs();
  removeFile(args.back());

  m_ffmpeg = std::make_unique<FFMPEG>(args);
}

void Convert::cancel()
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

void Convert::exportAsPython(const std::string& folder) const
{
  PythonScript script{ *this, folder };

  script.importOsPath();
  script.importSubprocess();

  script.newline();

  {
    script.def(script.fun(*this));

    std::vector<std::string> args = computeArgs();

    script.If(script.Not(script.osPathExists(script.str(args.back()))));

    script.CallFFmpeg(args);

    script.EndIf();

    script.enddef();
  }

  script.newline();

  script.callIfMain(script.fun(*this));
}

std::vector<std::string> Convert::computeArgs() const
{
  const Media& in = *input();

  QFileInfo file{ QString::fromStdString(in.name()) };
  const std::string output_file = (file.baseName() + "-converted." + file.suffix()).toStdString();

  std::vector<std::string> args;
  args.push_back("-i");
  args.push_back(in.name());

  args.insert(args.end(), m_opts.begin(), m_opts.end());

  args.push_back(output_file);

  return args;
}

void Convert::update()
{
  setProgress(100 * m_ffmpeg->progress());

  if (m_ffmpeg->isFinished())
  {
    setState(Job::DONE);
    m_ffmpeg = nullptr;
  }
}
