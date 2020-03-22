// Copyright (C) 2020 Vincent Chambrin
// This file is part of the 'ffmpeg-gui' project
// For conditions of distribution and use, see copyright notice in LICENSE

#include "media/audio.h"

Audio::Audio(int num, int samplerate)
  : Stream(num),
  m_samplerate(samplerate)
{

}

Stream::Kind Audio::kind() const
{
  return Stream::AUDIO;
}
