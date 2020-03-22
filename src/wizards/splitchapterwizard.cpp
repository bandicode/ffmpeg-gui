// Copyright (C) 2020 Vincent Chambrin
// This file is part of the 'ffmpeg-gui' project
// For conditions of distribution and use, see copyright notice in LICENSE

#include "splitchapterwizard.h"

#include "media.h"

#include "jobs/splitchapter.h"

#include <QLabel>
#include <QListWidget>


SplitChapterWizard::SplitChapterWizard(std::vector<std::shared_ptr<Media>> inputs, QWidget* parent)
  : Wizard(std::move(inputs), parent)
{
  contentWidget()->setLayout(new QVBoxLayout());
  contentWidget()->layout()->addWidget(new QLabel(
    "This wizard allow you to split a video file into its chapters."));

  QListWidget* list_widget = new QListWidget();

  for (const auto& chap : this->inputs().front()->chapters())
  {
    list_widget->addItem(QString::number(chap.num()) + " - " + QString::fromStdString(chap.title()));
  }

  contentWidget()->layout()->addWidget(list_widget);
}


std::string SplitChapterWizard::title()
{
  return "Split video by chapter";
}

std::string SplitChapterWizard::description()
{
  return "Creates a new video file for each chapter in the source file";
}

QIcon SplitChapterWizard::icon()
{
  return QIcon();
}

bool SplitChapterWizard::supports(const std::vector<std::shared_ptr<Media>>& inputs)
{
  return inputs.size() == 1 && !inputs.front()->chapters().empty();
}

Wizard* SplitChapterWizard::build(std::vector<std::shared_ptr<Media>> inputs)
{
  return new SplitChapterWizard(std::move(inputs));
}

std::unique_ptr<Job> SplitChapterWizard::create()
{
  return std::make_unique<SplitChapter>(nextJobNumber(), inputs().front());
}
