// Copyright (C) 2020 Vincent Chambrin
// This file is part of the 'ffmpeg-gui' project
// For conditions of distribution and use, see copyright notice in LICENSE

#ifndef FFMPEG_GUI_WIDGETS_STREAMSELECTORWIDGET_H
#define FFMPEG_GUI_WIDGETS_STREAMSELECTORWIDGET_H

#include <QListWidget>

#include <memory>
#include <vector>

class Media;
class Stream;

class StreamSelectorWidget : public QListWidget
{
  Q_OBJECT
public:
  explicit StreamSelectorWidget(QWidget* parent = nullptr);
  explicit StreamSelectorWidget(std::shared_ptr<Media> m, QWidget* parent = nullptr);

  void setMedia(std::shared_ptr<Media> m);

  std::shared_ptr<Media> media() const;
  std::vector<std::shared_ptr<Stream>> selectedStreams() const;

Q_SIGNALS:
  void selectionChanged();

protected Q_SLOTS:
  void onItemChanged(QListWidgetItem* item);

private:
  std::shared_ptr<Media> m_media;
};

#endif // FFMPEG_GUI_WIDGETS_STREAMSELECTORWIDGET_H
