// Copyright (C) 2020 Vincent Chambrin
// This file is part of the 'ffmpeg-gui' project
// For conditions of distribution and use, see copyright notice in LICENSE

#include "concatwizard.h"

#include "media.h"

#include "jobs/concat.h"

#include <QLabel>

ConcatWizard::ConcatWizard(std::vector<std::shared_ptr<Media>> inputs, QWidget* parent)
  : Wizard(std::move(inputs), parent)
{
  contentWidget()->setLayout(new QVBoxLayout());
  contentWidget()->layout()->addWidget(new QLabel(
    "This wizard allow you to concatenate multiple files into a single one."));
}

std::string ConcatWizard::title()
{
  return "Concatenate";
}

std::string ConcatWizard::description()
{
  return "Concatenate multiple files into a single one";
}

QIcon ConcatWizard::icon()
{
  return QIcon();
}

bool ConcatWizard::supports(const std::vector<std::shared_ptr<Media>>& inputs)
{
  return inputs.size() > 1;
}

Wizard* ConcatWizard::build(std::vector<std::shared_ptr<Media>> inputs)
{
  return new ConcatWizard(std::move(inputs));
}

std::unique_ptr<Job> ConcatWizard::create()
{
  return std::make_unique<Concat>(nextJobNumber(), inputs());
}
