// Copyright (C) 2020 Vincent Chambrin
// This file is part of the 'ffmpeg-gui' project
// For conditions of distribution and use, see copyright notice in LICENSE

#include "media/video.h"

Video::Video(int num, int w, int h, double fps)
  : Stream(num),
  m_width(w),
  m_height(h),
  m_fps(fps)
{

}

Stream::Kind Video::kind() const
{
  return Stream::VIDEO;
}