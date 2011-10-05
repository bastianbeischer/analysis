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
  , m_updateTimer(this)
  , m_selectedPlot(-1)
{
  m_layout->setContentsMargins(0, 0, 0, 0);
  s_rootWidget = new RootQtWidget(this);
  m_layout->addWidget(s_rootWidget);
  gROOT->cd();
  setMouseTracking(true);
  connect(&m_updateTimer, SIGNAL(timeout()), this, SLOT(update()));
  connect(s_rootWidget, SIGNAL(unzoomButtonPressed()), this, SLOT(unzoom()));
  connect(s_rootWidget, SIGNAL(positionChanged(double, double)), this, SLOT(canvasPositionChanged(double, double)));
  m_updateTimer.setInterval(500);
}

Plotter::~Plotter()
{
  qDeleteAll(m_plots);
}
  
QVector<AnalysisPlot*> Plotter::plots()
{
  return m_plots;
}

unsigned int Plotter::numberOfPlots()
{
  return m_plots.size();
}

void Plotter::canvasPositionChanged(double x, double y)
{
  if (m_selectedPlot < 0)
    return;
  
  m_plots[m_selectedPlot]->positionChanged(x,y);
  emit(positionChanged(x, y));
}

void Plotter::saveCanvas(const QString& fileName)
{
  s_rootWidget->GetCanvas()->SaveAs(qPrintable(fileName));
}

void Plotter::saveForPostAnalysis(const QString& fileName)
{
  TFile file(qPrintable(fileName), "RECREATE");
  for (unsigned int i = 0; i < numberOfPlots(); ++i) {
    AnalysisPlot* plot = m_plots.at(i);
    if (plot->isPlotCollection()) {
      static_cast<PlotCollection*>(plot)->saveForPostAnalysis(s_rootWidget->GetCanvas());
    } else {
      m_plots[i]->draw(s_rootWidget->GetCanvas());
      s_rootWidget->GetCanvas()->SetName(qPrintable(plotTitle(i) + " canvas"));
      s_rootWidget->GetCanvas()->Write();
    }
  }
  file.Close();
  selectPlot(m_selectedPlot);
  s_rootWidget->GetCanvas()->SetName("tqtwidget");
}

void Plotter::update()
{
  if (0 <= m_selectedPlot && m_selectedPlot < m_plots.size())
    m_plots[m_selectedPlot]->update();
  s_rootWidget->updateCanvas();
}

void Plotter::addPlot(AnalysisPlot* plot)
{
  m_plots.append(plot);
}

const QString& Plotter::plotTitle(unsigned int i)
{
  Q_ASSERT(i < numberOfPlots());
  return m_plots[i]->title();
}

QVector<unsigned int> Plotter::plotIndices(Enums::AnalysisTopic topic)
{
  QVector<unsigned int> ret;
  for (int i = 0; i < m_plots.size(); ++i)
    if (m_plots[i]->topic() == topic)
      ret.append(i);
  return ret;
}

Enums::AnalysisTopic Plotter::plotTopic(unsigned int i)
{
  Q_ASSERT(i < numberOfPlots());
  return m_plots[i]->topic();
}

void Plotter::selectPlot(int i)
{
  Q_ASSERT(i < int(numberOfPlots()));
  if (i < 0) {
    emit(titleChanged(QString()));
    emit(positionChanged(0, 0));
    gPad->Clear();
  } else {
    emit(titleChanged(m_plots[i]->title()));
    m_plots[i]->draw(s_rootWidget->GetCanvas());
    if (m_layout->count() > 1) {
      QWidget* prevWidget = m_layout->itemAt(0)->widget();
      m_layout->removeWidget(prevWidget);
      prevWidget->close();
    }
    QWidget* secondaryWidget = m_plots[i]->secondaryWidget();
    if (secondaryWidget) {
      m_layout->insertWidget(0, secondaryWidget);
      secondaryWidget->show();
    }
    s_rootWidget->updateCanvas();
  }
  m_selectedPlot = i;
}

void Plotter::clearPlots()
{
  qDeleteAll(m_plots);
  m_plots.clear();
}

void Plotter::finalizeAnalysis()
{
  foreach(AnalysisPlot* plot, m_plots) {
    plot->finalize();
    plot->update();
  }
  s_rootWidget->updateCanvas();
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
  if (m_selectedPlot < 0)
    return;
  m_plots[m_selectedPlot]->unzoom();
  s_rootWidget->updateCanvas();
}

RootPlot::Type Plotter::selectedPlotType()
{
  if (m_selectedPlot < 0) {
    return RootPlot::Undefined;
  }
  return m_plots[m_selectedPlot]->type();
}
  
RootPlot::DrawOption Plotter::drawOption()
{
  if (m_selectedPlot < 0) {
    return RootPlot::UndefinedDrawOption;
  }
  return m_plots[m_selectedPlot]->drawOption();
}
  
void Plotter::setDrawOption(RootPlot::DrawOption option)
{
  if (m_selectedPlot < 0)
    return;
  m_plots[m_selectedPlot]->setDrawOption(option);
  selectPlot(m_selectedPlot);
}

RootQtWidget* Plotter::rootWidget()
{
  return s_rootWidget;
}

void Plotter::setAspectRatio(double aspectRatio)
{
  s_rootWidget->setAspectRatio(aspectRatio);
}

