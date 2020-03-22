// Copyright (C) 2020 Vincent Chambrin
// This file is part of the 'ffmpeg-gui' project
// For conditions of distribution and use, see copyright notice in LICENSE

#ifndef FFMPEG_GUI_STREAM_H
#define FFMPEG_GUI_STREAM_H

#include <string>
#include <utility>
#include <vector>

using StreamMetadata = std::pair<std::string, std::string>;

class Stream
{
private:
  int m_num;
  std::vector<StreamMetadata> m_metadata;

public:
  virtual ~Stream();
  Stream& operator=(const Stream&) = delete;

  explicit Stream(int num);

  enum Kind
  {
    AUDIO,
    VIDEO,
    SUBTITLE,
  };

  virtual Kind kind() const = 0;

  int num() const;

  std::vector<StreamMetadata>& metadata();
  const std::vector<StreamMetadata>& metadata() const;

  bool metadata(const std::string& name, std::string& output) const noexcept;
  std::string metadata(const std::string& name) const;
};

inline int Stream::num() const
{
  return m_num;
}

inline std::vector<StreamMetadata>& Stream::metadata()
{
  return m_metadata;
}

inline const std::vector<StreamMetadata>& Stream::metadata() const
{
  return m_metadata;
}

#endif // FFMPEG_GUI_STREAM_H
