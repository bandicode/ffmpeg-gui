// Copyright (C) 2020 Vincent Chambrin
// This file is part of the 'ffmpeg-gui' project
// For conditions of distribution and use, see copyright notice in LICENSE

#ifndef FFMPEG_GUI_JOBS_CONVERT_H
#define FFMPEG_GUI_JOBS_CONVERT_H

#include "job.h"

#include <vector>

class Media;
class Stream;

class Convert : public Job
{
private:
  std::shared_ptr<Media> m_input;
  std::vector<std::string> m_opts;

public:
  Convert(int num, std::shared_ptr<Media> in, std::vector<std::string> opts);
  ~Convert();

  static const std::string ClassName;

  const std::string& kind() const override;

  const std::shared_ptr<Media>& input() const;

  void run() override;
  void cancel() override;

  void exportAsPython(const std::string& folder) const override;

protected:
  std::vector<std::string> computeArgs() const;
  void update() override;

private:
  std::unique_ptr<FFMPEG> m_ffmpeg;
};

inline const std::shared_ptr<Media>& Convert::input() const
{
  return m_input;
}

#endif // FFMPEG_GUI_JOBS_CONVERT_H
