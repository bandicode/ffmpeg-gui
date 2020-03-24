// Copyright (C) 2020 Vincent Chambrin
// This file is part of the 'ffmpeg-gui' project
// For conditions of distribution and use, see copyright notice in LICENSE

#include "joblistwidget.h"

#include "controller.h"
#include "ffmpeg.h"
#include "job.h"
#include "pythonscript.h"

#include <QProgressBar>
#include <QPushButton>

#include <QHBoxLayout>
#include <QVBoxLayout>

#include <QFileDialog>

#include <QDebug>

JobListWidget::JobListWidget(QWidget* parent)
  : QWidget(parent)
{
  connect(&Controller::instance(), &Controller::jobsChanged, this, &JobListWidget::onJobsChanged);

  QVBoxLayout* layout = new QVBoxLayout(this);

  m_joblist_widget = new QListWidget();
  layout->addWidget(m_joblist_widget);

  QHBoxLayout* sublayout = new QHBoxLayout;
  {
    m_progressbar = new QProgressBar;
    m_progressbar->setRange(0, 100);
    m_progressbar->setVisible(false);
    sublayout->addWidget(m_progressbar);

    m_button = new QPushButton("Run");
    sublayout->addWidget(m_button);

    m_export_button = new QPushButton("Export");
    sublayout->addWidget(m_export_button);

    m_clear_button = new QPushButton("Clear");
    sublayout->addWidget(m_clear_button);
  }
  layout->addLayout(sublayout);

  connect(m_button, &QPushButton::clicked, this, &JobListWidget::runJobs);
  connect(m_export_button, &QPushButton::clicked, this, &JobListWidget::exportJobs);
  connect(m_clear_button, &QPushButton::clicked, this, &JobListWidget::clearJobs);
}

void JobListWidget::onJobsChanged()
{
  m_joblist_widget->clear();

  for (const auto& job : Controller::instance().jobs())
  {
    m_joblist_widget->addItem(QString::fromStdString(std::to_string(job->num()) + " : " + job->kind()));
  }

  m_clear_button->setEnabled(!jobs().empty());
}

void JobListWidget::runJobs()
{
  disconnect(m_button, nullptr, this, nullptr);
  m_button->setText("Cancel");
  connect(m_button, &QPushButton::clicked, this, &JobListWidget::cancelJobs);

  processNextJob();
}

void JobListWidget::onJobStateChanged()
{
  Job* job = qobject_cast<Job*>(sender());

  if (job->state() == Job::DONE)
  {
    m_current_job++;

    updateProgress(static_cast<int>(m_current_job * 100 / jobs().size()));

    disconnect(job, nullptr, this, nullptr);
    processNextJob();
  }
}

const std::vector<std::unique_ptr<Job>>& JobListWidget::jobs() const
{
  return Controller::instance().jobs();
}

void JobListWidget::updateProgress(int n)
{
  if (n != -1)
  {
    m_progressbar->setEnabled(true);
    m_progressbar->setRange(0, 100);
    m_progressbar->setValue(n);
  }
}

void JobListWidget::cancelJobs()
{
  Job* job = jobs().at(m_current_job).get();
  job->cancel();
  disconnect(m_button, nullptr, this, nullptr);
  m_button->setText("Run");
  connect(m_button, &QPushButton::clicked, this, &JobListWidget::runJobs);
}

void JobListWidget::exportJobs()
{
  QString path = QFileDialog::getExistingDirectory(this, "Export jobs");

  if (path.isEmpty())
    return;

  std::string std_path = path.toStdString();
  PythonScript script{ std_path + "/jobs.py" };

  for (const auto& job : jobs())
  {
    job->exportAsPython(std_path);
    script.importJob(*job);
  }

  script.newline();

  for (const auto& job : jobs())
  {
    script.Call(script.fun(*job));
  }
}

void JobListWidget::clearJobs()
{
  Controller::instance().clearJobs();
}

void JobListWidget::processNextJob()
{
  if (m_current_job == Controller::instance().jobs().size())
  {
    disconnect(m_button, nullptr, this, nullptr);
    m_button->setText("Run");
    connect(m_button, &QPushButton::clicked, this, &JobListWidget::runJobs);
    Controller::instance().clearJobs();
  }

  Job* job = Controller::instance().jobs().at(m_current_job).get();
  connect(job, &Job::stateChanged, this, &JobListWidget::onJobStateChanged);
  job->run();
}
