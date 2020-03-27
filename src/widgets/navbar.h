// Copyright (C) 2020 Vincent Chambrin
// This file is part of the 'ffmpeg-gui' project
// For conditions of distribution and use, see copyright notice in LICENSE

#ifndef FFMPEG_GUI_WIDGETS_NAVBAR_H
#define FFMPEG_GUI_WIDGETS_NAVBAR_H

#include <QWidget>

class QPushButton;

class Window;

class NavbarWidget : public QWidget
{
  Q_OBJECT
public:
  explicit NavbarWidget(Window& window);

protected Q_SLOTS:
  void onPageOpened();
  void onPageClosed();
  void onHomeButtonClicked();

protected:
  QSize sizeHint() const override;

  void paintEvent(QPaintEvent* ev) override;

private:
  void recomputeNavbarSize();

private:
  QSize m_navbar_size;
  QPushButton* m_home_button;
};

#endif // FFMPEG_GUI_WIDGETS_NAVBAR_H
