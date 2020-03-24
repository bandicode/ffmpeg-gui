// Copyright (C) 2020 Vincent Chambrin
// This file is part of the 'ffmpeg-gui' project
// For conditions of distribution and use, see copyright notice in LICENSE

#include "controller.h"

#include "job.h"

#include <algorithm>

Controller* Controller::m_global_instance = nullptr;

Controller::Controller(QObject* parent)
  : QObject(parent)
{
  m_global_instance = this;
}

Controller::~Controller()
{
  m_global_instance = nullptr;
}

Controller& Controller::instance()
{
  return *m_global_instance;
}

void Controller::addInput(std::shared_ptr<Media> input)
{
  if (input)
  {
    m_inputs.push_back(input);
    Q_EMIT inputsChanged();
  }
}

void Controller::removeInput(std::shared_ptr<Media> input)
{
  auto it = std::find(m_inputs.begin(), m_inputs.end(), input);

  if (it != m_inputs.end())
  {
    m_inputs.erase(it);
    Q_EMIT inputsChanged();
  }
}

void Controller::removeInput(size_t index)
{
  m_inputs.erase(m_inputs.begin() + index);
  Q_EMIT inputsChanged();
}

std::vector<std::shared_ptr<Media>> Controller::takeInputs()
{
  std::vector<std::shared_ptr<Media>> ret{ std::move(m_inputs) };
  m_inputs.clear();
  Q_EMIT inputsChanged();
  return ret;
}

void Controller::addJob(std::unique_ptr<Job> job)
{
  if (job)
  {
    m_jobs.push_back(std::move(job));
    Q_EMIT jobsChanged();
  }
}

void Controller::removeJob(Job* job)
{
  auto it = std::find_if(m_jobs.begin(), m_jobs.end(), [job](const std::unique_ptr<Job>& j) {
    return j.get() == job; 
    });

  if (it != m_jobs.end())
  {
    m_jobs.erase(it);
    Q_EMIT jobsChanged();
  }
}

void Controller::removeJob(size_t index)
{
  m_jobs.erase(m_jobs.begin() + index);
  Q_EMIT jobsChanged();
}

void Controller::clearJobs()
{
  m_jobs.clear();
  Q_EMIT jobsChanged();
}
