// Copyright (C) 2020 Vincent Chambrin
// This file is part of the 'ffmpeg-gui' project
// For conditions of distribution and use, see copyright notice in LICENSE

#include "jobs/batchconvert.h"

#include "ffmpeg.h"

#include "media.h"
#include "media/stream.h"

#include "pythonscript.h"

#include <QFileInfo>

#include <QDebug>

const std::string BatchConvert::ClassName = "BatchConvert";

BatchConvert::BatchConvert(int num, std::vector<std::shared_ptr<Media>> ins, std::vector<std::string> opts)
  : Job(num),
  m_inputs(std::move(ins)), 
  m_opts(std::move(opts))
{
  setDescription("Converts multiple input files with the same options");
}

BatchConvert::~BatchConvert()
{

}

const std::string& BatchConvert::kind() const
{
  return ClassName;
}

void BatchConvert::run()
{
  if (state() == CANCELLED)
  {
    m_current_input = 0;
  }

  setState(RUNNING);

  processNextInput();
}

void BatchConvert::cancel()
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

void BatchConvert::exportAsPython(const std::string& folder) const
{
  PythonScript script{ *this, folder };

  script.importOsPath();
  script.importSubprocess();

  script.newline();

  {
    script.def(script.fun(*this));

    for (size_t i(0); i < inputs().size(); ++i)
    {
      std::vector<std::string> args = computeArgs(i);

      script.If(script.Not(script.osPathExists(script.str(args.back()))));

      script.CallFFmpeg(args);

      script.EndIf();
    }

    script.enddef();
  }

  script.newline();

  script.callIfMain(script.fun(*this));
}

std::vector<std::string> BatchConvert::computeArgs(size_t input_index) const
{
  const Media& input = *inputs().at(input_index);

  QFileInfo file{ QString::fromStdString(input.name()) };
  const std::string output_file = (file.baseName() + "-converted." + file.suffix()).toStdString();

  std::vector<std::string> args;
  args.push_back("-i");
  args.push_back(input.name());

  args.insert(args.end(), m_opts.begin(), m_opts.end());

  args.push_back(output_file);

  return args;
}

void BatchConvert::processNextInput()
{
  if (state() == CANCELLED)
  {
    return;
  }

  if (m_current_input == inputs().size())
  {
    setState(DONE);
    return;
  }

  std::vector<std::string> args = computeArgs(m_current_input);
  removeFile(args.back());

  m_ffmpeg = std::make_unique<FFMPEG>(args);
}

void BatchConvert::update()
{
  if (m_ffmpeg->isFinished())
  {
    m_current_input++;
    setProgress((static_cast<int>(m_current_input) - 1) * 100 / static_cast<int>(inputs().size()));
    m_ffmpeg = nullptr;
    processNextInput();
  }
}
