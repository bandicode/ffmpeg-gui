// Copyright (C) 2020 Vincent Chambrin
// This file is part of the 'ffmpeg-gui' project
// For conditions of distribution and use, see copyright notice in LICENSE

#ifndef FFMPEG_GUI_VIDEO_H
#define FFMPEG_GUI_VIDEO_H

#include "stream.h"

class Video : public Stream
{
  std::pair<int, int> m_size;
  std::pair<int, int> m_sar;
  std::pair<int, int> m_dar;
  double m_fps;

public:

  Video(int num, std::pair<int, int> size, std::pair<int, int> sar, std::pair<int, int> dar, double fps);

  Kind kind() const override;

  int width() const;
  int height() const;

  const std::pair<int, int>& size() const;
  const std::pair<int, int>& sar() const;
  const std::pair<int, int>& dar() const;

  double fps() const;
};

inline int Video::width() const
{
  return size().first;
}

inline int Video::height() const
{
  return size().second;
}

inline double Video::fps() const
{
  return m_fps;
}

inline const std::pair<int, int>& Video::size() const
{
  return m_size;
}

inline const std::pair<int, int>& Video::sar() const
{
  return m_sar;
}

inline const std::pair<int, int>& Video::dar() const
{
  return m_dar;
}

#endif // FFMPEG_GUI_VIDEO_H
