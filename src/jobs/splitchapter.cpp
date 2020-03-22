// Copyright (C) 2020 Vincent Chambrin
// This file is part of the 'ffmpeg-gui' project
// For conditions of distribution and use, see copyright notice in LICENSE

#include "jobs/splitchapter.h"

#include "media.h"

#include <QFileInfo>
#include <QProcess>

#include <QDebug>

const std::string SplitChapter::ClassName = "SplitChapter";

SplitChapter::SplitChapter(int num, std::shared_ptr<Media> media)
  : Job(num),
  m_input(media)
{
  setDescription("Split a video file based on its chapters.");
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
  if (m_process)
  {
    if (m_process->state() == QProcess::Running)
    {
      m_process->kill();
    }
  }

  setState(CANCELLED);
}

void SplitChapter::exportAsPython(const std::string& folder) const
{

}

void SplitChapter::processNextChapter()
{
  if (state() == CANCELLED)
  {
    return;
  }

  if (m_current_chapter == m_input->chapters().size())
  {
    setState(DONE);
    return;
  }

  m_process = new QProcess(this);
  m_process->setProgram("ffmpeg");

  const Chapter& chap = m_input->chapters().at(m_current_chapter);

  const std::string start = std::to_string(chap.start());
  const std::string end = std::to_string(chap.end());

  QFileInfo file{ QString::fromStdString(m_input->name()) };

  QStringList args;
  args << "-i" << QString::fromStdString(m_input->name())
    << "-vcodec" << "copy"
    << "-acodec" << "copy"
    << "-ss" << QString::fromStdString(start)
    << "-to" << QString::fromStdString(end)
    << (file.baseName() + "-chap-" + QString::number(chap.num()) + "." + file.suffix());

  qDebug() << args;

  m_process->setArguments(args);

  connect(m_process, &QProcess::stateChanged, this, &SplitChapter::onProcessStateChanged);
  connect(m_process, &QProcess::readyReadStandardError, this, &SplitChapter::onProcessReadyReadStandardError);
  connect(m_process, static_cast<void(QProcess::*)(int, QProcess::ExitStatus)>(&QProcess::finished), this, &SplitChapter::onProcessFinished);

  m_process->setReadChannel(QProcess::StandardError);
  m_process->start();
}

void SplitChapter::onProcessStateChanged()
{

}

void SplitChapter::onProcessReadyReadStandardError()
{
  while (m_process->canReadLine())
  {
    qDebug() << m_process->readLine();
  }
}

void SplitChapter::onProcessFinished(int exitCode)
{
  m_current_chapter++;
  setProgress((m_current_chapter - 1) * 100 / m_input->chapters().size());
  m_process->deleteLater();
  m_process = nullptr;

  processNextChapter();
}
