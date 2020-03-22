// Copyright (C) 2020 Vincent Chambrin
// This file is part of the 'ffmpeg-gui' project
// For conditions of distribution and use, see copyright notice in LICENSE

#ifndef FFMPEG_GUI_AUDIO_H
#define FFMPEG_GUI_AUDIO_H

#include "stream.h"

class Audio : public Stream
{
private:
  int m_samplerate;

public:

  Audio(int num, int samplerate);

  Kind kind() const override;

  int samplerate() const;
};

inline int Audio::samplerate() const
{
  return m_samplerate;
}

#endif // FFMPEG_GUI_AUDIO_H
