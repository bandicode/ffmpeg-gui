// Copyright (C) 2020 Vincent Chambrin
// This file is part of the 'ffmpeg-gui' project
// For conditions of distribution and use, see copyright notice in LICENSE

#ifndef FFMPEG_GUI_WIDGETS_JOBLISTWIDGET_H
#define FFMPEG_GUI_WIDGETS_JOBLISTWIDGET_H

#include <QListWidget>

class QProgressBar;
class QPushButton;

class Job;

class JobListWidget : public QWidget
{
  Q_OBJECT
public:
  explicit JobListWidget(QWidget* parent = nullptr);

protected Q_SLOTS:
  void onJobsChanged();
  void runJobs();
  void onJobStateChanged();
  void cancelJobs();
  void exportJobs();
  void clearJobs();

protected:
  const std::vector<std::unique_ptr<Job>>& jobs() const;
  void updateProgress(int n);
  void processNextJob();

private:
  QListWidget* m_joblist_widget;
  QProgressBar* m_progressbar;
  QPushButton* m_button;
  QPushButton* m_export_button;
  QPushButton* m_clear_button;
  size_t m_current_job = 0;
};

#endif // FFMPEG_GUI_WIDGETS_JOBLISTWIDGET_H
