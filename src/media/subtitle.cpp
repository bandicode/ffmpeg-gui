// Copyright (C) 2020 Vincent Chambrin
// This file is part of the 'ffmpeg-gui' project
// For conditions of distribution and use, see copyright notice in LICENSE

#include "media/subtitle.h"

Subtitle::Subtitle(int num)
  : Stream(num)
{

}

Stream::Kind Subtitle::kind() const
{
  return Stream::SUBTITLE;
}
