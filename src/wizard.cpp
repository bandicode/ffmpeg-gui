// Copyright (C) 2020 Vincent Chambrin
// This file is part of the 'ffmpeg-gui' project
// For conditions of distribution and use, see copyright notice in LICENSE

#include "wizard.h"

#include "controller.h"
#include "job.h"

#include <QProgressBar>
#include <QPushButton>

JobRunnerWidget::JobRunnerWidget(std::unique_ptr<Job> job)
  : m_job(std::move(job))
{

  m_progress_bar = new QProgressBar;
  m_progress_bar->setEnabled(false);
  m_button = new QPushButton("Starting");
  m_button->setEnabled(false);

  QHBoxLayout* l = new QHBoxLayout(this);
  l->addWidget(m_progress_bar, 1);
  l->addWidget(m_button);

  connect(m_job.get(), &Job::stateChanged, this, &JobRunnerWidget::onStateChanged);

  m_job->run();
}

JobRunnerWidget::~JobRunnerWidget()
{

}

void JobRunnerWidget::onStateChanged()
{
  if (m_job->state() == Job::RUNNING)
  {
    m_button->setText("Cancel");
    m_button->setEnabled(true);
    connect(m_button, &QPushButton::clicked, this, &JobRunnerWidget::cancelJob);
    connect(m_job.get(), &Job::progressChanged, this, &JobRunnerWidget::updateProgress);
  }
  else if (m_job->state() == Job::CANCELLED)
  {
    m_button->setText("Close");
    m_button->setEnabled(true);
    connect(m_button, &QPushButton::clicked, this, &JobRunnerWidget::requestClose);
  }
  else if (m_job->state() == Job::DONE)
  {
    updateProgress(100);
    disconnect(m_button, nullptr, this, nullptr);
    m_button->setText("Done!");
    connect(m_button, &QPushButton::clicked, this, &JobRunnerWidget::requestClose);
  }
}

void JobRunnerWidget::updateProgress(int n)
{
  if (n != -1)
  {
    m_progress_bar->setEnabled(true);
    m_progress_bar->setRange(0, 100);
    m_progress_bar->setValue(n);
  }
}

void JobRunnerWidget::cancelJob()
{
  disconnect(m_button, nullptr, this, nullptr);
  m_button->setText("Cancelling");
  m_button->setEnabled(false);
  m_job->cancel();
}

void JobRunnerWidget::requestClose()
{
  disconnect(m_button, nullptr, this, nullptr);
  Q_EMIT closeRequested();
}

Wizard::Wizard(std::vector<std::shared_ptr<Media>> inputs, QWidget* parent)
  : Page(parent),
  m_inputs(std::move(inputs))
{
  QVBoxLayout* layout = new QVBoxLayout(this);

  m_content = new QWidget();
  layout->addWidget(m_content);

  {
    m_buttons_layout = new QHBoxLayout();
    m_buttons_layout->addStretch();

    m_run_button = new QPushButton("Run");
    m_queue_button = new QPushButton("Cancel");

    m_buttons_layout->addWidget(m_run_button);
    m_buttons_layout->addWidget(m_queue_button);

    m_buttons_layout->addStretch();

    layout->addLayout(m_buttons_layout);
  }

  connect(runButton(), &QPushButton::clicked, this, &Wizard::createAndRun);
  connect(queueButton(), &QPushButton::clicked, this, &Wizard::createAndQueue);
}

Wizard::~Wizard()
{

}

void Wizard::createAndRun()
{
  m_run_button->setVisible(false);
  m_queue_button->setVisible(false);

  auto job = create();
  auto* runner_widget = new JobRunnerWidget(std::move(job));

  connect(runner_widget, &JobRunnerWidget::closeRequested, this, &Wizard::close);

  layout()->addWidget(runner_widget);
}

void Wizard::createAndQueue()
{
  auto job = create();
  Controller::instance().addJob(std::move(job));
  close();
}

QWidget* Wizard::contentWidget()
{
  return m_content;
}

QPushButton* Wizard::runButton() const
{
  return m_run_button;
}

QPushButton* Wizard::queueButton() const
{
  return m_queue_button;
}

int Wizard::nextJobNumber()
{
  static int num = 0;
  return ++num;
}

void Wizard::setCreationButtonsEnabled(bool on)
{
  m_run_button->setEnabled(on);
  m_queue_button->setEnabled(on);
}

WizardPrototype::~WizardPrototype()
{

}
