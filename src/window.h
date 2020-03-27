// Copyright (C) 2020 Vincent Chambrin
// This file is part of the 'ffmpeg-gui' project
// For conditions of distribution and use, see copyright notice in LICENSE

#ifndef FFMPEG_GUI_WINDOW_H
#define FFMPEG_GUI_WINDOW_H

#include <QWidget>

#include <memory>

class Controller;
class Page;

class Window : public QWidget
{
  Q_OBJECT
public:
  explicit Window(Controller& con);
  ~Window();

  static Window& instance();

  Controller& controller() const;

  void open(Page* page);
  void closePage();

  const std::vector<Page*>& pages() const;

Q_SIGNALS:
  void opened(Page* page);
  void closed(Page* page);

protected:

protected Q_SLOTS:

private:
  static Window* m_global_instance;

private:
  Controller& m_controller;
  std::vector<Page*> m_pages;
};

inline Controller& Window::controller() const
{
  return m_controller;
}

inline const std::vector<Page*>& Window::pages() const
{
  return m_pages;
}

#endif // FFMPEG_GUI_WINDOW_H
