// Copyright (C) 2020 Vincent Chambrin
// This file is part of the 'ffmpeg-gui' project
// For conditions of distribution and use, see copyright notice in LICENSE

#include "controller.h"
#include "window.h"

#include <QApplication>

int main(int argc, char* argv[])
{
  QApplication app{ argc, argv };

  Controller con;

  Window master_windu{ con };
  master_windu.show();

  return app.exec();
}
