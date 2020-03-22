// Copyright (C) 2020 Vincent Chambrin
// This file is part of the 'ffmpeg-gui' project
// For conditions of distribution and use, see copyright notice in LICENSE

#ifndef FFMPEG_GUI_CHAPTER_H
#define FFMPEG_GUI_CHAPTER_H

#include <string>

class Chapter
{
private:
  int m_num;
  double m_start;
  double m_end;
  std::string m_title;

public:
  Chapter(int num, double start, double end, std::string title = "");

  int num() const;
  double start() const;
  double end() const;
  std::string& title();
  const std::string& title() const;
};

inline int Chapter::num() const
{
  return m_num;
}

inline double Chapter::start() const
{
  return m_start;
}

inline double Chapter::end() const
{
  return m_end;
}

inline std::string& Chapter::title()
{
  return m_title;
}

inline const std::string& Chapter::title() const
{
  return m_title;
}

#endif // FFMPEG_GUI_CHAPTER_H
