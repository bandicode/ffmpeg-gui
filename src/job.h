// Copyright (C) 2020 Vincent Chambrin
// This file is part of the 'ffmpeg-gui' project
// For conditions of distribution and use, see copyright notice in LICENSE

#ifndef FFMPEG_GUI_JOB_H
#define FFMPEG_GUI_JOB_H

#include <QObject>

#include <string>

class FFMPEG;

class Job : public QObject
{
  Q_OBJECT
public:
  virtual ~Job();
  Job& operator=(const Job&) = delete;

  explicit Job(int num);

  virtual const std::string& kind() const = 0;

  int num() const;

  const std::string& description() const;

  enum State
  {
    WAITING,
    STARTED,
    RUNNING,
    CANCELLED,
    DONE,
  };

  enum Result
  {
    SUCCESS,
    FAILURE,
  };

  int progress() const;
  
  State state() const;

  const std::string& status() const;

  Result result() const;

  virtual void run() = 0;
  virtual void cancel() = 0;

  virtual void exportAsPython(const std::string& folder) const = 0;

Q_SIGNALS:
  void stateChanged();
  void progressChanged(int n);
  void statusChanged();

protected:
  void setDescription(std::string des);
  void setState(State s);
  void setProgress(int p);
  void setStatus(std::string mssg);
  void setResult(Result r);

  bool userRemove(const std::string& file_path);
  void remove(const std::string& file_path);

  virtual void update() = 0;

  void timerEvent(QTimerEvent* ev) override;

private:
  int m_num;
  std::string m_description;
  State m_state = WAITING;
  std::string m_status;
  int m_progress = -1;
  Result m_result = FAILURE;
};

inline int Job::num() const
{
  return m_num;
}

inline const std::string& Job::description() const
{
  return m_description;
}

inline Job::State Job::state() const
{
  return m_state;
}

inline const std::string& Job::status() const
{
  return m_status;
}

inline int Job::progress() const
{
  return m_progress;
}

inline Job::Result Job::result() const
{
  return m_result;
}

#endif // FFMPEG_GUI_JOB_H
