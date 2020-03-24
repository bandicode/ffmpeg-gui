// Copyright (C) 2020 Vincent Chambrin
// This file is part of the 'ffmpeg-gui' project
// For conditions of distribution and use, see copyright notice in LICENSE

#ifndef FFMPEG_GUI_JOBS_BATCHCONVERT_H
#define FFMPEG_GUI_JOBS_BATCHCONVERT_H

#include "job.h"

#include <vector>

class Media;
class Stream;

class BatchConvert : public Job
{
private:
  std::vector<std::shared_ptr<Media>> m_inputs;
  std::vector<std::string> m_opts;

public:
  BatchConvert(int num, std::vector<std::shared_ptr<Media>> ins, std::vector<std::string> opts);
  ~BatchConvert();

  static const std::string ClassName;

  const std::string& kind() const override;

  const std::vector<std::shared_ptr<Media>>& inputs() const;

  void run() override;
  void cancel() override;

  void exportAsPython(const std::string& folder) const override;

protected:
  std::vector<std::string> computeArgs(size_t input_index) const;

  void processNextInput();

  void update() override;

private:
  size_t m_current_input = 0;
  std::unique_ptr<FFMPEG> m_ffmpeg;
};

inline const std::vector<std::shared_ptr<Media>>& BatchConvert::inputs() const
{
  return m_inputs;
}

#endif // FFMPEG_GUI_JOBS_BATCHCONVERT_H
