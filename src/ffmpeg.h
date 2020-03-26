// Copyright (C) 2020 Vincent Chambrin
// This file is part of the 'ffmpeg-gui' project
// For conditions of distribution and use, see copyright notice in LICENSE

#ifndef FFMPEG_GUI_FFMPEG_H
#define FFMPEG_GUI_FFMPEG_H

#include <memory>
#include <string>
#include <vector>

class QPixmap;
class QProcess;

struct Crop;

class Media;

class FFMPEG
{
public:
  FFMPEG();
  explicit FFMPEG(const std::vector<std::string>& args);
  ~FFMPEG();

  enum ExitStatus
  {
    NORMAL_EXIT = 0,
    CRASH_EXIT = 1,
  };

  bool isRunning() const;
  bool isFinished() const;
  void kill();
  void waitForFinished();
  ExitStatus exitStatus() const;
  int exitCode() const;

  double progress() const;

  const std::string& output() const;

  static std::string version();
  static std::shared_ptr<Media> info(const std::string& path);
  static Crop cropdetect(const Media& media, int nbsamples = 10);
  static QPixmap snapshot(const Media& media, double time);

  QProcess* qprocess() const;

protected:
  void update();

private:
  QProcess* m_process = nullptr;
  double m_input_duration = 0.f;
  double m_progress = 0.;
  std::string m_output;
  bool m_duration_parsed = false;
  bool m_enconding_started = false;
};

inline double FFMPEG::progress() const
{
  return m_progress;
}

inline const std::string& FFMPEG::output() const
{
  return m_output;
}

inline QProcess* FFMPEG::qprocess() const
{
  return m_process;
}

#endif // FFMPEG_GUI_FFMPEG_H
