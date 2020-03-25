// Copyright (C) 2020 Vincent Chambrin
// This file is part of the 'ffmpeg-gui' project
// For conditions of distribution and use, see copyright notice in LICENSE

#include "media/video.h"

Video::Video(int num, std::pair<int, int> size, std::pair<int, int> sar, std::pair<int, int> dar, double fps)
  : Stream(num),
  m_size(size),
  m_sar(sar),
  m_dar(dar),
  m_fps(fps)
{

}

Stream::Kind Video::kind() const
{
  return Stream::VIDEO;
}