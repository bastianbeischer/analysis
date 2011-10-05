#include "Plotter.hh"
#include "AnalysisPlot.hh"
#include "PlotCollection.hh"

#include <QApplication>
#include <QVBoxLayout>
#include <QDebug>

#include <TPad.h>
#include <TROOT.h>
#include <TFile.h>

#include "RootQtWidget.hh"

#include <iostream>

RootQtWidget* Plotter::s_rootWidget = 0;

Plotter::Plotter(QWidget* parent)
  : QWidget(parent)
  , m_layout(new QVBoxLayout(this))
  , m_plot(0)
  , m_updateTimer(this)
{
  m_layout->setContentsMargins(0, 0, 0, 0);
  s_rootWidget = new RootQtWidget(this);
  m_layout->addWidget(s_rootWidget);
  setLayout(m_layout);
  setMouseTracking(true);
  connect(&m_updateTimer, SIGNAL(timeout()), this, SLOT(update()));
  connect(s_rootWidget, SIGNAL(unzoomButtonPressed()), this, SLOT(unzoom()));
  connect(s_rootWidget, SIGNAL(positionChanged(double, double)), this, SLOT(canvasPositionChanged(double, double)));
  m_updateTimer.setInterval(500);
}

Plotter::~Plotter()
{
}
  
void Plotter::canvasPositionChanged(double x, double y)
{
  if (!m_plot)
    return;
  m_plot->positionChanged(x, y);
  emit(positionChanged(x, y));
}

void Plotter::saveCanvas(const QString& fileName)
{
  s_rootWidget->GetCanvas()->SaveAs(qPrintable(fileName));
}

void Plotter::update()
{
  if (m_plot)
    m_plot->update();
  s_rootWidget->updateCanvas();
}

AnalysisPlot* Plotter::plot()
{
  return m_plot;
}

void Plotter::setPlot(AnalysisPlot* plot)
{
  m_plot = plot;

  if (m_layout->count() > 1) {
    QWidget* prevWidget = m_layout->itemAt(0)->widget();
    prevWidget->close();
    m_layout->removeWidget(prevWidget);
    prevWidget->setParent(0);
  }

  if (m_plot) {
    m_plot->draw(s_rootWidget->GetCanvas());
    QWidget* secondaryWidget = m_plot->secondaryWidget();
    if (secondaryWidget) {
      m_layout->insertWidget(0, secondaryWidget);
      secondaryWidget->show();
    }
    s_rootWidget->updateCanvas();
  } else {
    emit(positionChanged(0, 0));
    gPad->Clear();
  }

  update();
}

void Plotter::toggleUpdateTimer()
{
  if (m_updateTimer.isActive())
    m_updateTimer.stop();
  else
    m_updateTimer.start();
}

void Plotter::unzoom()
{
  if (!m_plot)
    return;
  m_plot->unzoom();
  s_rootWidget->updateCanvas();
}

RootQtWidget* Plotter::rootWidget()
{
  return s_rootWidget;
}

void Plotter::setAspectRatio(double aspectRatio)
{
  s_rootWidget->setAspectRatio(aspectRatio);
}
