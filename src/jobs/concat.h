// Copyright (C) 2020 Vincent Chambrin
// This file is part of the 'ffmpeg-gui' project
// For conditions of distribution and use, see copyright notice in LICENSE

#ifndef FFMPEG_GUI_JOBS_CONCAT_H
#define FFMPEG_GUI_JOBS_CONCAT_H

#include "job.h"

#include <vector>

class Media;
class Stream;

class Concat : public Job
{
private:
  std::vector<std::shared_ptr<Media>> m_inputs;

public:
  Concat(int num, std::vector<std::shared_ptr<Media>> ins);
  ~Concat();

  static const std::string ClassName;

  const std::string& kind() const override;

  const std::vector<std::shared_ptr<Media>>& inputs() const;

  void run() override;
  void cancel() override;

  void exportAsPython(const std::string& folder) const override;

protected:
  std::string createInputListFile(const std::string& folder) const;
  std::vector<std::string> computeArgs(const std::string& inputlistfile) const;

  void update() override;

private:
  std::unique_ptr<FFMPEG> m_ffmpeg;
};

inline const std::vector<std::shared_ptr<Media>>& Concat::inputs() const
{
  return m_inputs;
}

#endif // FFMPEG_GUI_JOBS_CONCAT_H
