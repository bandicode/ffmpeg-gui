// Copyright (C) 2020 Vincent Chambrin
// This file is part of the 'ffmpeg-gui' project
// For conditions of distribution and use, see copyright notice in LICENSE

#include "batchconvertwizard.h"

#include "media.h"

#include "jobs/batchconvert.h"

#include <QLabel>
#include <QLineEdit>

BatchConvertWizard::BatchConvertWizard(std::vector<std::shared_ptr<Media>> inputs, QWidget* parent)
  : Wizard(QString::fromStdString(title()), std::move(inputs), parent)
{
  contentWidget()->setLayout(new QVBoxLayout());
  contentWidget()->layout()->addWidget(new QLabel(
    "This wizard allow you to convert multiple files with the same ffmpeg flags."));

  m_options_linedit = new QLineEdit(this);
  m_options_linedit->setPlaceholderText("specify ffmpeg flags");

  contentWidget()->layout()->addWidget(m_options_linedit);
}

std::string BatchConvertWizard::title()
{
  return "Batch conversion";
}

std::string BatchConvertWizard::description()
{
  return "Converts multiple files with the same flags";
}

QIcon BatchConvertWizard::icon()
{
  return QIcon();
}

bool BatchConvertWizard::supports(const std::vector<std::shared_ptr<Media>>& inputs)
{
  return inputs.size() > 1;
}

Wizard* BatchConvertWizard::build(std::vector<std::shared_ptr<Media>> inputs)
{
  return new BatchConvertWizard(std::move(inputs));
}

std::unique_ptr<Job> BatchConvertWizard::create()
{
  QStringList args = m_options_linedit->text().split(" ");
  std::vector<std::string> opts;

  for (auto a : args)
    opts.push_back(a.toStdString());

  return std::make_unique<BatchConvert>(nextJobNumber(), inputs(), std::move(opts));
}
