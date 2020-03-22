// Copyright (C) 2020 Vincent Chambrin
// This file is part of the 'ffmpeg-gui' project
// For conditions of distribution and use, see copyright notice in LICENSE

#include "inputlistwidget.h"

#include "controller.h"
#include "ffmpeg.h"
#include "media.h"

#include <QDragEnterEvent>
#include <QDragLeaveEvent>
#include <QDropEvent>

#include <QMimeData>
#include <QUrl>

#include <QDebug>

InputListWidget::InputListWidget(QWidget* parent)
  : QListWidget(parent)
{
  setAcceptDrops(true);

  connect(&Controller::instance(), &Controller::inputsChanged, this, &InputListWidget::onInputsChanged);
}

void InputListWidget::onInputsChanged()
{
  clear();

  for (auto media : Controller::instance().inputs())
  {
    addItem(QString::fromStdString(media->title() + " : " + media->name()));
  }
}

void InputListWidget::dragEnterEvent(QDragEnterEvent* ev)
{
  if (ev->mimeData()->urls().size() != 1)
  {
    ev->ignore();
    return;
  }

  try
  {
    QUrl url = ev->mimeData()->urls().first();
    QString path = url.toLocalFile();
    auto info = FFMPEG::info(path.toStdString());
    ev->accept();
  }
  catch (...)
  {
    ev->ignore();
  }
}

void InputListWidget::dragMoveEvent(QDragMoveEvent* ev)
{
  ev->accept();
}

void InputListWidget::dragLeaveEvent(QDragLeaveEvent* ev)
{
  ev->accept();
}

void InputListWidget::dropEvent(QDropEvent* ev)
{
  QUrl url = ev->mimeData()->urls().first();
  QString path = url.toLocalFile();
  auto info = FFMPEG::info(path.toStdString());
 
  Controller::instance().addInput(info);
}
