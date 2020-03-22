// Copyright (C) 2020 Vincent Chambrin
// This file is part of the 'ffmpeg-gui' project
// For conditions of distribution and use, see copyright notice in LICENSE

#ifndef FFMPEG_GUI_WIDGETS_WIZARDLISTWIDGET_H
#define FFMPEG_GUI_WIDGETS_WIZARDLISTWIDGET_H

#include <QListWidget>

#include "wizard.h"

class WizardListWidget : public QListWidget
{
  Q_OBJECT
public:
  explicit WizardListWidget(QWidget* parent = nullptr);

protected Q_SLOTS:
  void onInputsChanged();
  void onItemDoubleClicked(QListWidgetItem* item);

private:
  std::vector<std::unique_ptr<WizardPrototype>> m_prototypes;
};

#endif // FFMPEG_GUI_WIDGETS_WIZARDLISTWIDGET_H
