// Copyright (C) 2020 Vincent Chambrin
// This file is part of the 'ffmpeg-gui' project
// For conditions of distribution and use, see copyright notice in LICENSE

#ifndef FFMPEG_GUI_CONTROLLER_H
#define FFMPEG_GUI_CONTROLLER_H

#include <QObject>

#include <memory>
#include <vector>

class Job;
class Media;

class Controller : public QObject
{
  Q_OBJECT
public:
  explicit Controller(QObject* parent = nullptr);
  ~Controller();

  static Controller& instance();

  const std::vector<std::shared_ptr<Media>>& inputs() const;

  void addInput(std::shared_ptr<Media> input);
  void removeInput(std::shared_ptr<Media> input);
  void removeInput(size_t index);
  std::vector<std::shared_ptr<Media>> takeInputs();

  const std::vector<std::unique_ptr<Job>>& jobs() const;

  void addJob(std::unique_ptr<Job> job);
  void removeJob(Job* job);
  void removeJob(size_t index);
  void clearJobs();

protected Q_SLOTS:

Q_SIGNALS:
  void inputsChanged();
  void jobsChanged();

private:
  static Controller* m_global_instance;

private:
  std::vector<std::shared_ptr<Media>> m_inputs;
  std::vector<std::unique_ptr<Job>> m_jobs;
};

inline const std::vector<std::shared_ptr<Media>>& Controller::inputs() const
{
  return m_inputs;
}

inline const std::vector<std::unique_ptr<Job>>& Controller::jobs() const
{
  return m_jobs;
}

#endif // FFMPEG_GUI_CONTROLLER_H
