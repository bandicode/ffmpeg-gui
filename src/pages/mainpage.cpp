// Copyright (C) 2020 Vincent Chambrin
// This file is part of the 'ffmpeg-gui' project
// For conditions of distribution and use, see copyright notice in LICENSE

#include "mainpage.h"

#include "widgets/inputlistwidget.h"
#include "widgets/wizardlistwidget.h"

#include <QDesktopServices> 
#include <QLabel>
#include <QVBoxLayout>
#include <QUrl>

MainPage::MainPage(QWidget* parent)
  : Page(parent)
{
  QVBoxLayout* layout = new QVBoxLayout(this);

  auto* label = new QLabel;
  label->setTextFormat(Qt::RichText);
  label->setText(
    "<h1>ffmpeg GUI</h1>"
    "Welcome to ffmpeg GUI, a graphical user interface for the ffmpeg software. <br/>"
    "If you want to learn more about ffmpeg, click <a href=\"https://www.ffmpeg.org/\">here</a>."
  );

  layout->addWidget(label);

  m_inputs_widget = new InputListWidget;
  layout->addWidget(m_inputs_widget);

  m_wizards_widget = new WizardListWidget;
  layout->addWidget(m_wizards_widget);

  connect(label, &QLabel::linkActivated, this, &MainPage::onUrlClicked);
}

MainPage::~MainPage()
{

}

void MainPage::onUrlClicked(const QString& url)
{
  if (url.startsWith("https"))
  {
    QDesktopServices::openUrl(QUrl::fromUserInput(url));
  }
}
