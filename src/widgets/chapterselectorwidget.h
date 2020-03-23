// Copyright (C) 2020 Vincent Chambrin
// This file is part of the 'ffmpeg-gui' project
// For conditions of distribution and use, see copyright notice in LICENSE

#ifndef FFMPEG_GUI_WIDGETS_CHAPTERSELECTORWIDGET_H
#define FFMPEG_GUI_WIDGETS_CHAPTERSELECTORWIDGET_H

#include <QListWidget>

#include <memory>
#include <vector>

class Media;
class Stream;

class ChapterSelectorWidget : public QListWidget
{
  Q_OBJECT
public:
  explicit ChapterSelectorWidget(QWidget* parent = nullptr);
  explicit ChapterSelectorWidget(std::shared_ptr<Media> m, QWidget* parent = nullptr);

  void setMedia(std::shared_ptr<Media> m);

  std::shared_ptr<Media> media() const;
  std::vector<size_t> selectedChapters() const;

Q_SIGNALS:
  void selectionChanged();

protected Q_SLOTS:
  void onItemChanged(QListWidgetItem* item);

private:
  std::shared_ptr<Media> m_media;
};

#endif // FFMPEG_GUI_WIDGETS_STREAMSELECTORWIDGET_H
