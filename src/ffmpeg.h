// Copyright (C) 2020 Vincent Chambrin
// This file is part of the 'ffmpeg-gui' project
// For conditions of distribution and use, see copyright notice in LICENSE

#ifndef FFMPEG_GUI_FFMPEG_H
#define FFMPEG_GUI_FFMPEG_H

#include <memory>
#include <string>

class Media;

class FFMPEG
{
public:

  static std::string version();

  static std::shared_ptr<Media> info(const std::string& path);
};

#endif // FFMPEG_GUI_FFMPEG_H
