// Copyright (C) 2020 Vincent Chambrin
// This file is part of the 'ffmpeg-gui' project
// For conditions of distribution and use, see copyright notice in LICENSE

#include "window.h"

#include "controller.h"
#include "ffmpeg.h"
#include "media.h"

#include "pages/mainpage.h"

#include <QIcon>

#include <QHBoxLayout>
#include <QVBoxLayout>

#include <QDebug>

Window* Window::m_global_instance = nullptr;

Window::Window(Controller& c)
  : m_controller(c)
{
  m_global_instance = this;

  std::string ver = FFMPEG::version();

  setWindowTitle("ffmpeg GUI - " + QString::fromStdString(ver));
  setWindowIcon(QIcon(":/logo.svg"));

  QVBoxLayout* layout = new QVBoxLayout(this);
  m_pages.push_back(new MainPage());
  layout->addWidget(m_pages.back());
}

Window::~Window()
{
  m_pages.clear();

  m_global_instance = nullptr;
}

Window& Window::instance()
{
  return *m_global_instance;
}

void Window::open(Page* page)
{
  layout()->itemAt(layout()->count() - 1)->widget()->hide();
  layout()->addWidget(page);
  m_pages.push_back(page);
}

void Window::closePage()
{
  layout()->removeWidget(layout()->itemAt(layout()->count() - 1)->widget());
  m_pages.back()->deleteLater();
  m_pages.pop_back();
  layout()->itemAt(layout()->count() - 1)->widget()->show();
}
