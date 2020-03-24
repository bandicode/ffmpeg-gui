// Copyright (C) 2020 Vincent Chambrin
// This file is part of the 'ffmpeg-gui' project
// For conditions of distribution and use, see copyright notice in LICENSE

#include "wizardlistwidget.h"

#include "controller.h"
#include "window.h"

#include "wizards/batchconvertwizard.h"
#include "wizards/concatwizard.h"
#include "wizards/splitchapterwizard.h"

WizardListWidget::WizardListWidget(QWidget* parent)
  : QListWidget(parent)
{
  m_prototypes.push_back(WizardPrototype::make<BatchConvertWizard>());
  m_prototypes.push_back(WizardPrototype::make<ConcatWizard>());
  m_prototypes.push_back(WizardPrototype::make<SplitChapterWizard>());

  connect(&Controller::instance(), &Controller::inputsChanged, this, &WizardListWidget::onInputsChanged);
  connect(this, &QListWidget::itemDoubleClicked, this, &WizardListWidget::onItemDoubleClicked);

  this->setVisible(false);
}

void WizardListWidget::onInputsChanged()
{
  clear();

  const auto& inputs = Controller::instance().inputs();

  for (const auto& wiz : m_prototypes)
  {
    if (wiz->supports(inputs))
    {
      addItem(QString::fromStdString(wiz->title()));
    }
  }

  setVisible(count() != 0);
}

void WizardListWidget::onItemDoubleClicked(QListWidgetItem* item)
{
  for (const auto& wiz : m_prototypes)
  {
    if (wiz->title() == item->text().toStdString())
    {
      Window::instance().open(wiz->build(Controller::instance().takeInputs()));
      return;
    }
  }
}
