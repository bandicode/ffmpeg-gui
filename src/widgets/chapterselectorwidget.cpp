// Copyright (C) 2020 Vincent Chambrin
// This file is part of the 'ffmpeg-gui' project
// For conditions of distribution and use, see copyright notice in LICENSE

#include "chapterselectorwidget.h"

#include "media.h"
#include "media/stream.h"

ChapterSelectorWidget::ChapterSelectorWidget(QWidget* parent)
  : ChapterSelectorWidget(nullptr, parent)
{
  
}

ChapterSelectorWidget::ChapterSelectorWidget(std::shared_ptr<Media> m, QWidget* parent)
  : QListWidget(parent)
{
  if (m)
    setMedia(m);

  connect(this, &QListWidget::itemChanged, this, &ChapterSelectorWidget::onItemChanged);
}

void ChapterSelectorWidget::setMedia(std::shared_ptr<Media> m)
{
  clear();

  m_media = m;

  if (!m)
    return;

  for (const auto& chap : m->chapters())
  {
    QListWidgetItem* item = new QListWidgetItem(QString::number(chap.num()) + " - " + QString::fromStdString(chap.title()));
    item->setFlags(Qt::ItemIsEnabled | Qt::ItemIsUserCheckable);
    item->setCheckState(Qt::Checked);
    addItem(item);
  }

  Q_EMIT selectionChanged();
}

std::shared_ptr<Media> ChapterSelectorWidget::media() const
{
  return m_media;
}

std::vector<size_t> ChapterSelectorWidget::selectedChapters() const
{
  std::vector<size_t> ret;

  for (int i(0); i < count(); ++i)
  {
    if (item(i)->checkState() == Qt::Checked)
      ret.push_back(static_cast<size_t>(i));
  }

  return ret;
}

void ChapterSelectorWidget::onItemChanged(QListWidgetItem* item)
{
  Q_EMIT selectionChanged();
}
