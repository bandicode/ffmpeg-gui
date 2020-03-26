// Copyright (C) 2020 Vincent Chambrin
// This file is part of the 'ffmpeg-gui' project
// For conditions of distribution and use, see copyright notice in LICENSE

#ifndef FFMPEG_GUI_MEDIA_CROP_H
#define FFMPEG_GUI_MEDIA_CROP_H

#include <string>

struct Crop
{
  int width;
  int height;
  int x;
  int y;
};

inline bool operator==(const Crop& lhs, const Crop& rhs)
{
  return lhs.width == rhs.width && lhs.height == rhs.height && lhs.x == rhs.x && lhs.y == rhs.y;
}

inline bool operator!=(const Crop& lhs, const Crop& rhs)
{
  return !(lhs == rhs);
}

inline std::string to_string(const Crop& c)
{
  using std::to_string;
  return to_string(c.width) + ":" + to_string(c.height) + ":" + to_string(c.x) + ":" + to_string(c.y);
}

#endif // FFMPEG_GUI_MEDIA_CROP_H
