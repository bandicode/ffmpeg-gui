// Copyright (C) 2020 Vincent Chambrin
// This file is part of the 'ffmpeg-gui' project
// For conditions of distribution and use, see copyright notice in LICENSE

#include "media/stream.h"

#include <algorithm>
#include <stdexcept>

Stream::Stream(int num)
  : m_num(num)
{

}

Stream::~Stream()
{

}

bool Stream::metadata(const std::string& name, std::string& output) const noexcept
{
  auto it = std::find_if(m_metadata.begin(), m_metadata.end(), [&name](const StreamMetadata& md) -> bool {
    return md.first == name;
    });

  if (it != m_metadata.end())
    output = it->second;

  return it != m_metadata.end();
}

std::string Stream::metadata(const std::string& name) const
{
  auto it = std::find_if(m_metadata.begin(), m_metadata.end(), [&name](const StreamMetadata& md) -> bool {
    return md.first == name;
    });

  if (it == m_metadata.end())
  {
    throw std::runtime_error{ "Stream::metadata() : no such metadata" };
  }

  return it->second;
}