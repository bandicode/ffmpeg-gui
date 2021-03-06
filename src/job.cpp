// Copyright (C) 2020 Vincent Chambrin
// This file is part of the 'ffmpeg-gui' project
// For conditions of distribution and use, see copyright notice in LICENSE

#include "job.h"

#include <QTimerEvent>

#include <QFile>
#include <QFileInfo>

#include <algorithm>
#include <stdexcept>

Job::Job(int num)
  : m_num(num)
{

}

Job::~Job()
{

}

void Job::setDescription(std::string des)
{
  m_description = std::move(des);
}

void Job::setState(State s)
{
  if (m_state != s)
  {
    m_state = s;
    Q_EMIT stateChanged();

    if (m_state == State::RUNNING)
    {
      startTimer(50);
    }
  }
}

void Job::setProgress(int p)
{
  if (m_progress != p)
  {
    m_progress = p;
    Q_EMIT progressChanged(p);
  }
}

void Job::setStatus(std::string mssg)
{
  if (m_status != mssg)
  {
    m_status = std::move(mssg);
    Q_EMIT statusChanged();
  }
}

void Job::setResult(Result r)
{
  m_result = r;
  setState(DONE);
}

bool Job::userRemove(const std::string& file_path)
{
  // TODO: ask user if necessary
  removeFile(file_path);

  return !QFileInfo::exists(QString::fromStdString(file_path));
}

void Job::removeFile(const std::string& file_path)
{
  if (QFileInfo::exists(QString::fromStdString(file_path)))
  {
    QFile::remove(QString::fromStdString(file_path));
  }
}

void Job::timerEvent(QTimerEvent* ev)
{
  ev->accept();

  if (state() != State::RUNNING)
  {
    killTimer(ev->timerId());
  }
  else
  {
    update();
  }
}
