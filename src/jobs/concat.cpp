// Copyright (C) 2020 Vincent Chambrin
// This file is part of the 'ffmpeg-gui' project
// For conditions of distribution and use, see copyright notice in LICENSE

#include "jobs/concat.h"

#include "ffmpeg.h"

#include "media.h"
#include "media/stream.h"

#include "pythonscript.h"

#include <QDir>
#include <QFileInfo>

#include <QDebug>

const std::string Concat::ClassName = "Concat";

Concat::Concat(int num, std::vector<std::shared_ptr<Media>> ins)
  : Job(num),
  m_inputs(std::move(ins))
{
  setDescription("Concatenates multiple input files into a single one");
}

Concat::~Concat()
{

}

const std::string& Concat::kind() const
{
  return ClassName;
}

void Concat::run()
{
  setState(RUNNING);

  std::string temp_folder = QDir::tempPath().toStdString();
  std::string path = createInputListFile(temp_folder);

  std::vector<std::string> args = computeArgs(path);
  removeFile(args.back());

  m_ffmpeg = std::make_unique<FFMPEG>(args);
}

void Concat::cancel()
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

void Concat::exportAsPython(const std::string& folder) const
{
  std::string list_file = createInputListFile(folder);

  PythonScript script{ *this, folder };

  script.importOsPath();
  script.importSubprocess();

  script.newline();

  {
    script.def(script.fun(*this));

    std::vector<std::string> args = computeArgs(list_file);
    script.If(script.Not(script.osPathExists(script.str(args.back()))));
    script.CallFFmpeg(args);
    script.EndIf();

    script.enddef();
  }

  script.newline();

  script.callIfMain(script.fun(*this));
}

std::string Concat::createInputListFile(const std::string& folder) const
{
  std::string path = folder + "/concat-job-" + std::to_string(num()) + ".list";
  std::ofstream output_file{ path };

  for (const auto& in : inputs())
  {
    output_file << "file '" << in->name() << "'\n";
  }

  output_file.close();

  return path;
}

std::vector<std::string> Concat::computeArgs(const std::string& inputlistfile) const
{
  const Media& input = *inputs().front();

  QFileInfo file{ QString::fromStdString(input.name()) };
  const std::string output_file = (file.baseName() + "-concatenated." + file.suffix()).toStdString();

  std::vector<std::string> args;
  args.push_back("-f");
  args.push_back("concat");
  args.push_back("-i");
  args.push_back(inputlistfile);
  args.push_back("-c");
  args.push_back("copy");  
  args.push_back("-fflags");
  args.push_back("+genpts");

  args.push_back(output_file);

  return args;
}

void Concat::update()
{
  if (m_ffmpeg->isFinished())
  {
    setProgress(100);
    m_ffmpeg = nullptr;
  }
  else
  {
    setProgress(static_cast<int>(m_ffmpeg->progress() * 100));
  }
}
