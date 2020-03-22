// Copyright (C) 2020 Vincent Chambrin
// This file is part of the 'ffmpeg-gui' project
// For conditions of distribution and use, see copyright notice in LICENSE

#ifndef FFMPEG_GUI_MEDIA_H
#define FFMPEG_GUI_MEDIA_H

#include <memory>
#include <string>
#include <vector>

#include "media/chapter.h"

class Stream;

class Media
{
private:
  std::string m_name;
  std::string m_title;
  std::vector<Chapter> m_chapters;
  std::vector<std::shared_ptr<Stream>> m_streams;

public:
  explicit Media(std::string name, std::string title = "");
  ~Media();

  const std::string& name() const;

  std::string& title();
  const std::string& title() const;
  
  std::vector<Chapter>& chapters();
  const std::vector<Chapter>& chapters() const;

  std::vector<std::shared_ptr<Stream>>& streams();
  const std::vector<std::shared_ptr<Stream>>& streams() const;
};

inline const std::string& Media::name() const
{
  return m_name;
}

inline std::string& Media::title()
{
  return m_title;
}

inline const std::string& Media::title() const
{
  return m_title;
}

inline std::vector<Chapter>& Media::chapters()
{
  return m_chapters;
}

inline const std::vector<Chapter>& Media::chapters() const
{
  return m_chapters;
}

inline std::vector<std::shared_ptr<Stream>>& Media::streams()
{
  return m_streams;
}

inline const std::vector<std::shared_ptr<Stream>>& Media::streams() const
{
  return m_streams;
}

#endif // FFMPEG_GUI_MEDIA_H
