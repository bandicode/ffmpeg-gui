// Copyright (C) 2020 Vincent Chambrin
// This file is part of the 'ffmpeg-gui' project
// For conditions of distribution and use, see copyright notice in LICENSE

#include "page.h"

#include "window.h"

Page::Page(QWidget* parent)
  : QWidget(parent)
{

}

Page::~Page()
{

}

void Page::closePage()
{
  Window::instance().closePage();
}

void Page::close()
{
  closePage();
}
