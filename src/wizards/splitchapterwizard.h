// Copyright (C) 2020 Vincent Chambrin
// This file is part of the 'ffmpeg-gui' project
// For conditions of distribution and use, see copyright notice in LICENSE

#ifndef FFMPEG_GUI_WIZARDS_SPLITCHAPTER_H
#define FFMPEG_GUI_WIZARDS_SPLITCHAPTER_H

#include "wizard.h"

class ChapterSelectorWidget;
class StreamSelectorWidget;

class SplitChapterWizard : public Wizard
{
  Q_OBJECT
public:
  explicit SplitChapterWizard(std::vector<std::shared_ptr<Media>> inputs, QWidget* parent = nullptr);

  static std::string title();
  static std::string description();
  static QIcon icon();
  static bool supports(const std::vector<std::shared_ptr<Media>>& inputs);
  static Wizard* build(std::vector<std::shared_ptr<Media>> inputs);

protected:
  std::unique_ptr<Job> create() override;

private:
  ChapterSelectorWidget* m_chapter_selector_widget;
  StreamSelectorWidget* m_stream_selector_widget;
};

#endif // FFMPEG_GUI_WIZARDS_SPLITCHAPTER_H
