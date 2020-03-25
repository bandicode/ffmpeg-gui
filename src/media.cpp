// Copyright (C) 2020 Vincent Chambrin
// This file is part of the 'ffmpeg-gui' project
// For conditions of distribution and use, see copyright notice in LICENSE

#include "media.h"

#include "media/stream.h"
#include "media/video.h"

Media::Media(std::string name, std::string title)
  : m_name(std::move(name)),
  m_title(std::move(title))
{

}

Media::~Media()
{

}

bool Media::hasVideo() const
{
  return video() != nullptr;
}

std::shared_ptr<Video> Media::video() const
{
  for (const auto& s : streams())
  {
    if (s->kind() == Stream::VIDEO)
    {
      return std::static_pointer_cast<Video>(s);
    }
  }

  return nullptr;
}
