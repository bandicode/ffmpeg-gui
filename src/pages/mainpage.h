// Copyright (C) 2020 Vincent Chambrin
// This file is part of the 'ffmpeg-gui' project
// For conditions of distribution and use, see copyright notice in LICENSE

#ifndef FFMPEG_GUI_PAGES_MAINPAGE_H
#define FFMPEG_GUI_PAGES_MAINPAGE_H

#include "page.h"

class InputListWidget;
class JobListWidget;
class WizardListWidget;

class MainPage : public Page
{
  Q_OBJECT
public:
  explicit MainPage(QWidget* parent = nullptr);
  ~MainPage();

protected Q_SLOTS:
  void onUrlClicked(const QString& url);
  void onInputsChanged();
  void onJobsChanged();

private:
  InputListWidget* m_inputs_widget;
  JobListWidget* m_jobs_widget;
  WizardListWidget* m_wizards_widget;
};

#endif // FFMPEG_GUI_PAGES_MAINPAGE_H
