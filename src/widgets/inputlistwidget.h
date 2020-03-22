// Copyright (C) 2020 Vincent Chambrin
// This file is part of the 'ffmpeg-gui' project
// For conditions of distribution and use, see copyright notice in LICENSE

#ifndef FFMPEG_GUI_WIDGETS_INPUTLISTWIDGET_H
#define FFMPEG_GUI_WIDGETS_INPUTLISTWIDGET_H

#include <QListWidget>

class InputListWidget : public QListWidget
{
  Q_OBJECT
public:
  explicit InputListWidget(QWidget* parent = nullptr);

protected Q_SLOTS:
  void onInputsChanged();

protected:
  void dragEnterEvent(QDragEnterEvent* ev) override;
  void dragMoveEvent(QDragMoveEvent* ev) override;
  void dragLeaveEvent(QDragLeaveEvent* ev) override;
  void dropEvent(QDropEvent* ev) override;
};

#endif // FFMPEG_GUI_WIDGETS_INPUTLISTWIDGET_H
