// Copyright (C) 2020 Vincent Chambrin
// This file is part of the 'ffmpeg-gui' project
// For conditions of distribution and use, see copyright notice in LICENSE

#include "convertwizard.h"

#include "media.h"
#include "media/crop.h"

#include "ffmpeg.h"

#include "jobs/convert.h"

#include <QLabel>
#include <QLineEdit>
#include <QPixmap>
#include <QPushButton>

#include <QMessageBox>

ConvertWizard::ConvertWizard(std::vector<std::shared_ptr<Media>> inputs, QWidget* parent)
  : Wizard(QString::fromStdString(title()), std::move(inputs), parent)
{
  contentWidget()->setLayout(new QVBoxLayout());
  contentWidget()->layout()->addWidget(new QLabel(
    "This wizard allow you to convert multiple files with the same ffmpeg flags."));

  m_detectcrop_button = new QPushButton("Detect crop");
  m_detectcrop_button->setEnabled(this->inputs().front()->hasVideo());
  contentWidget()->layout()->addWidget(m_detectcrop_button);

  if (this->inputs().front()->hasVideo())
  {
    try
    {
      QPixmap pix = FFMPEG::snapshot(*this->inputs().front(), this->inputs().front()->duration() / 2.);
      QLabel* label = new QLabel();
      label->setPixmap(pix);
      contentWidget()->layout()->addWidget(label);
    }
    catch (...)
    {

    }
  }

  m_options_linedit = new QLineEdit(this);
  m_options_linedit->setPlaceholderText("specify ffmpeg flags");

  contentWidget()->layout()->addWidget(m_options_linedit);

  connect(m_detectcrop_button, &QPushButton::clicked, this, &ConvertWizard::detectCrop);
}

std::string ConvertWizard::title()
{
  return "Convert";
}

std::string ConvertWizard::description()
{
  return "Convert a single file";
}

QIcon ConvertWizard::icon()
{
  return QIcon();
}

bool ConvertWizard::supports(const std::vector<std::shared_ptr<Media>>& inputs)
{
  return inputs.size() == 1;
}

Wizard* ConvertWizard::build(std::vector<std::shared_ptr<Media>> inputs)
{
  return new ConvertWizard(std::move(inputs));
}

void ConvertWizard::detectCrop()
{
  try
  {
    Crop crop = FFMPEG::cropdetect(*inputs().front());
    QMessageBox::information(this, "Crop detect", QString::fromStdString(to_string(crop)), QMessageBox::Ok);
  }
  catch (...)
  {
    QMessageBox::warning(this, "Crop detect", "Could not detect crop", QMessageBox::Ok);
  }
}

std::unique_ptr<Job> ConvertWizard::create()
{
  QStringList args = m_options_linedit->text().split(" ");
  std::vector<std::string> opts;

  for (auto a : args)
    opts.push_back(a.toStdString());

  return std::make_unique<Convert>(nextJobNumber(), inputs().front(), std::move(opts));
}
