// Copyright (C) 2020 Vincent Chambrin
// This file is part of the 'ffmpeg-gui' project
// For conditions of distribution and use, see copyright notice in LICENSE


#include "ffmpeg.h"

#include "media.h"

#include "parser.h"

#include <QDebug>
#include <QProcess>
#include <QRegularExpression>

#include <stdexcept>

std::string FFMPEG::version()
{
  QProcess proc;
  proc.start("ffmpeg");

  if (!proc.waitForFinished())
    throw std::runtime_error{ "FFMPEG::version() : timeout" };

  QByteArray ba = proc.readAllStandardError();

  QRegularExpression regexp{ "ffmpeg version (\\d)\\.(\\d)\\.(\\d) Copyright" };
  QRegularExpressionMatch match = regexp.match(QString::fromUtf8(ba));

  if (!match.hasMatch()) 
    throw std::runtime_error{ "FFMPEG::version() : could not fetch version" };

  return (match.captured(1) + "." + match.captured(2) + "." + match.captured(3)).toStdString();
}

std::shared_ptr<Media> FFMPEG::info(const std::string& path)
{
  QProcess proc;
  proc.start("ffmpeg", QStringList{"-hide_banner", "-i", QString::fromStdString(path)});

  if (!proc.waitForFinished())
    throw std::runtime_error{ "FFMPEG::info() : timeout" };

  QByteArray ba = proc.readAllStandardError();

  Parser parser;
  return parser.parseMediaInfo(ba.toStdString());
}