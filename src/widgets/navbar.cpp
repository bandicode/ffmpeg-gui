// Copyright (C) 2020 Vincent Chambrin
// This file is part of the 'ffmpeg-gui' project
// For conditions of distribution and use, see copyright notice in LICENSE

#include "navbar.h"

#include "page.h"
#include "window.h"

#include <QCoreApplication>

#include <QSize>

#include <QBrush>
#include <QFontMetrics>
#include <QPainter>
#include <QPen>
#include <QPaintEvent>

#include <QPushButton>

#include <QHBoxLayout>

#include <QDebug>

static const int PEN_WIDTH = 2;
static const int ARROW_HEIGHT = 12;
static const int VERTICAL_MARGIN = 2;
static const int LEFT_MARGIN = 6;
static const int RIGHT_MARGIN = 0;

NavbarWidget::NavbarWidget(Window& window)
  : QWidget(&window),
  m_navbar_size(0, 0)
{
  connect(&window, &Window::opened, this, &NavbarWidget::onPageOpened);
  connect(&window, &Window::closed, this, &NavbarWidget::onPageClosed);

  auto* layout = new QHBoxLayout;
  layout->setContentsMargins(0, 0, 6, 0);
  layout->addStretch();
  setLayout(layout);

  m_home_button = new QPushButton;
  m_home_button->setIcon(QIcon(":/assets/home.svg"));
  m_home_button->setFlat(true);
  m_home_button->setFixedSize(16, 16);
  layout->addWidget(m_home_button);

  connect(m_home_button, &QPushButton::clicked, this, &NavbarWidget::onHomeButtonClicked);
}

void NavbarWidget::onPageOpened()
{
  recomputeNavbarSize();

  m_home_button->setVisible(Window::instance().pages().size() > 1);
}

void NavbarWidget::onPageClosed()
{
  recomputeNavbarSize();

  m_home_button->setVisible(Window::instance().pages().size() > 1);
}

void NavbarWidget::onHomeButtonClicked()
{
  Window& w = Window::instance();

  while (w.pages().size() > 1)
  {
    w.closePage();
  }

  update();
}

QSize NavbarWidget::sizeHint() const
{
  QSize s = QWidget::sizeHint();

  s.rheight() = std::max(s.height(), m_navbar_size.height());
  s.rwidth() = s.width() + m_navbar_size.width();

  return s;
}

void NavbarWidget::paintEvent(QPaintEvent* ev)
{
  QWidget::paintEvent(ev);

  Window& w = Window::instance();

  if (w.pages().empty())
    return;

  QPainter painter{ this };
  painter.setRenderHint(QPainter::RenderHint::Antialiasing, true);
  painter.setRenderHint(QPainter::RenderHint::TextAntialiasing, true);

  QPen border_pen;
  border_pen.setWidth(PEN_WIDTH);
  border_pen.setColor(QColor{"#adacb5"});
  border_pen.setJoinStyle(Qt::MiterJoin);
  
  QPen text_pen;
  text_pen.setColor(Qt::black);

  painter.setBrush(Qt::NoBrush);

  QFontMetrics metrics{ font() };

  int x_offset = 0;

  for (Page* p : w.pages())
  {
    QPainterPath path;
    path.moveTo(x_offset - ARROW_HEIGHT, PEN_WIDTH / 2);

    const int w = metrics.width(p->title());
    path.lineTo(x_offset + w + LEFT_MARGIN + RIGHT_MARGIN, PEN_WIDTH / 2);

    path.lineTo(x_offset + w + LEFT_MARGIN + RIGHT_MARGIN + ARROW_HEIGHT, PEN_WIDTH / 2 + height() / 2);
    path.lineTo(x_offset + w + LEFT_MARGIN + RIGHT_MARGIN, height() - PEN_WIDTH / 2);

    path.lineTo(x_offset - ARROW_HEIGHT, height() - PEN_WIDTH / 2);

    painter.setPen(border_pen);
    painter.drawPath(path);

    const auto text_rect = QRect(QPoint(x_offset + LEFT_MARGIN, 0), QSize(w, height()));
    painter.setPen(text_pen);
    painter.drawText(text_rect, Qt::AlignCenter, p->title());

    x_offset += w + ARROW_HEIGHT + LEFT_MARGIN + RIGHT_MARGIN;
  }
}

void NavbarWidget::recomputeNavbarSize()
{
  Window& w = Window::instance();
  QFontMetrics metrics{ font() };

  m_navbar_size = QSize(0, metrics.height() + 2 * PEN_WIDTH + 2* VERTICAL_MARGIN);

  for (Page* p : w.pages())
  {
    m_navbar_size.rwidth() += ARROW_HEIGHT + metrics.width(p->title()) + 2 * LEFT_MARGIN;
  }

  setFixedHeight(m_navbar_size.height());

  layout()->invalidate();
}
