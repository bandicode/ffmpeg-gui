// Copyright (C) 2020 Vincent Chambrin
// This file is part of the 'ffmpeg-gui' project
// For conditions of distribution and use, see copyright notice in LICENSE

#ifndef FFMPEG_GUI_VIDEO_H
#define FFMPEG_GUI_VIDEO_H

#include "stream.h"

class Video : public Stream
{
  int m_width;
  int m_height;
  double m_fps;

public:

  Video(int num, int w, int h, double fps);

  Kind kind() const override;

  int width() const;
  int height() const;

  double fps() const;
};

inline int Video::width() const
{
  return m_width;
}

inline int Video::height() const
{
  return m_height;
}

inline double Video::fps() const
{
  return m_fps;
}

#endif // FFMPEG_GUI_VIDEO_H
