// Copyright (C) 2020 Vincent Chambrin
// This file is part of the 'ffmpeg-gui' project
// For conditions of distribution and use, see copyright notice in LICENSE

#ifndef FFMPEG_GUI_WIZARD_H
#define FFMPEG_GUI_WIZARD_H

#include "page.h"

#include <QHBoxLayout>
#include <QVBoxLayout>

#include <QIcon>

#include <memory>
#include <vector>

class QProgressBar;
class QPushButton;

class Job;
class Media;

class JobRunnerWidget : public QWidget
{
  Q_OBJECT
public:
  explicit JobRunnerWidget(std::unique_ptr<Job> job);
  ~JobRunnerWidget();

Q_SIGNALS:
  void closeRequested();

protected Q_SLOTS:
  void onStateChanged();
  void updateProgress(int n);
  void cancelJob();
  void requestClose();

private:
  std::unique_ptr<Job> m_job;
  QProgressBar* m_progress_bar;
  QPushButton* m_button;
};

class Wizard : public Page
{
  Q_OBJECT
public:
  explicit Wizard(std::vector<std::shared_ptr<Media>> inputs, QWidget* parent = nullptr);
  ~Wizard();

  const std::vector<std::shared_ptr<Media>>& inputs() const;

protected Q_SLOTS:
  void createAndRun();
  void createAndQueue();

protected:
  QWidget* contentWidget();
  QPushButton* runButton() const;
  QPushButton* queueButton() const;

  int nextJobNumber();
  virtual std::unique_ptr<Job> create() = 0;

  void setCreationButtonsEnabled(bool on = true);

private:
  std::vector<std::shared_ptr<Media>> m_inputs;
  QWidget* m_content;
  QHBoxLayout* m_buttons_layout;
  QPushButton* m_run_button;
  QPushButton* m_queue_button;
};

class WizardPrototype
{
public:
  virtual ~WizardPrototype();

  virtual std::string title() const = 0;
  virtual std::string description() const = 0;
  virtual QIcon icon() const = 0;
  virtual bool supports(const std::vector<std::shared_ptr<Media>>& inputs) const = 0;
  virtual Wizard* build(std::vector<std::shared_ptr<Media>> inputs) const = 0;

  template<typename T>
  static std::unique_ptr<WizardPrototype> make();
};

template<typename T>
struct WizardPrototypeImpl : public WizardPrototype
{
  std::string title() const override
  {
    return T::title();
  }

  std::string description() const
  {
    return T::description();
  }

  QIcon icon() const
  {
    return T::icon();
  }

  bool supports(const std::vector<std::shared_ptr<Media>>& inputs) const
  {
    return T::supports(inputs);
  }

  Wizard* build(std::vector<std::shared_ptr<Media>> inputs) const
  {
    return T::build(std::move(inputs));
  }
};

inline const std::vector<std::shared_ptr<Media>>& Wizard::inputs() const
{
  return m_inputs;
}

template<typename T>
inline std::unique_ptr<WizardPrototype> WizardPrototype::make()
{
  return std::make_unique<WizardPrototypeImpl<T>>();
}

#endif // FFMPEG_GUI_WIZARD_H
