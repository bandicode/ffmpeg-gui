// Copyright (C) 2020 Vincent Chambrin
// This file is part of the 'ffmpeg-gui' project
// For conditions of distribution and use, see copyright notice in LICENSE

#ifndef FFMPEG_GUI_JOBS_SPLITCHAPTER_H
#define FFMPEG_GUI_JOBS_SPLITCHAPTER_H

#include "job.h"

class QProcess;

class Media;

class SplitChapter : public Job
{
private:
  std::shared_ptr<Media> m_input;

public:

  SplitChapter(int num, std::shared_ptr<Media> media, std::vector<size_t> which_chapters);
  ~SplitChapter();

  static const std::string ClassName;

  const std::string& kind() const override;

  const std::shared_ptr<Media>& input() const;

  void run() override;
  void cancel() override;

  void exportAsPython(const std::string& folder) const override;

protected:
  std::vector<std::string> computeArgs(size_t chap_index) const;

  void processNextChapter();

  void update() override;

private:
  std::vector<size_t> m_chapters;
  size_t m_current_chapter = 0;
  std::unique_ptr<FFMPEG> m_ffmpeg;
};

inline const std::shared_ptr<Media>& SplitChapter::input() const
{
  return m_input;
}

#endif // FFMPEG_GUI_JOBS_SPLITCHAPTER_H
