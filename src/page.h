// Copyright (C) 2020 Vincent Chambrin
// This file is part of the 'ffmpeg-gui' project
// For conditions of distribution and use, see copyright notice in LICENSE

#ifndef FFMPEG_GUI_PAGE_H
#define FFMPEG_GUI_PAGE_H

#include <QWidget>

class Page : public QWidget
{
  Q_OBJECT
public:
  explicit Page(QWidget* parent = nullptr);
  ~Page();

protected:
  void closePage();
  void close(); // hides QWidget::close()

};

#endif // FFMPEG_GUI_PAGE_H
