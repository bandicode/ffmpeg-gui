// Copyright (C) 2020 Vincent Chambrin
// This file is part of the 'ffmpeg-gui' project
// For conditions of distribution and use, see copyright notice in LICENSE

#include "job.h"

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
