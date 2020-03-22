// Copyright (C) 2020 Vincent Chambrin
// This file is part of the 'ffmpeg-gui' project
// For conditions of distribution and use, see copyright notice in LICENSE

#include "media/chapter.h"

Chapter::Chapter(int num, double start, double end, std::string title)
  : m_num(num),
  m_start(start),
  m_end(end),
  m_title(std::move(title))
{

}
