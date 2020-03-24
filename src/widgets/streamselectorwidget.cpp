// Copyright (C) 2020 Vincent Chambrin
// This file is part of the 'ffmpeg-gui' project
// For conditions of distribution and use, see copyright notice in LICENSE

#include "streamselectorwidget.h"

#include "media.h"
#include "media/stream.h"

StreamSelectorWidget::StreamSelectorWidget(QWidget* parent)
  : StreamSelectorWidget(nullptr, parent)
{
  
}

StreamSelectorWidget::StreamSelectorWidget(std::shared_ptr<Media> m, QWidget* parent)
  : QListWidget(parent)
{
  if (m)
    setMedia(m);

  connect(this, &QListWidget::itemChanged, this, &StreamSelectorWidget::onItemChanged);
}

void StreamSelectorWidget::setMedia(std::shared_ptr<Media> m)
{
  clear();

  m_media = m;

  if (!m)
    return;

  for (const auto& s : m->streams())
  {
    QString text = "Stream " + QString::number(s->num()) + " " + QString::fromStdString(to_string(s->kind()));
    QListWidgetItem* item = new QListWidgetItem(text);
    item->setFlags(Qt::ItemIsEnabled | Qt::ItemIsUserCheckable);
    item->setCheckState(Qt::Checked);
    addItem(item);
  }

  Q_EMIT selectionChanged();
}

std::shared_ptr<Media> StreamSelectorWidget::media() const
{
  return m_media;
}

std::vector<std::shared_ptr<Stream>> StreamSelectorWidget::selectedStreams() const
{
  std::vector<std::shared_ptr<Stream>> ret;

  for (int i(0); i < count(); ++i)
  {
    if (item(i)->checkState() == Qt::Checked)
      ret.push_back(m_media->streams().at(static_cast<size_t>(i)));
  }

  return ret;
}

void StreamSelectorWidget::onItemChanged(QListWidgetItem* item)
{
  Q_EMIT selectionChanged();
}
