// Copyright (C) 2020 Vincent Chambrin
// This file is part of the 'ffmpeg-gui' project
// For conditions of distribution and use, see copyright notice in LICENSE

#ifndef FFMPEG_GUI_SUBTITLE_H
#define FFMPEG_GUI_SUBTITLE_H

#include "stream.h"

class Subtitle : public Stream
{
public:
  explicit Subtitle(int num);

  Kind kind() const override;
};

#endif // FFMPEG_GUI_SUBTITLE_H
