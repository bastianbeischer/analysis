#include "Plotter.hh"
#include "AnalysisPlot.hh"
#include "H2DPlot.hh"

#include <QApplication>
#include <QVBoxLayout>
#include <QDebug>

#include <TPad.h>
#include <TROOT.h>
#include <TFile.h>

#include <TQtWidget.h>

#include <iostream>

TQtWidget* Plotter::s_rootWidget = 0;

Plotter::Plotter(QWidget* parent)
  : QWidget(parent)
  , m_layout(new QVBoxLayout)
  , m_updateTimer(this)
  , m_selectedPlot(-1)
{
  s_rootWidget = new TQtWidget;
  m_layout->addWidget(s_rootWidget);
  setLayout(m_layout);
  gROOT->cd();
  setMouseTracking(true);
  connect(&m_updateTimer, SIGNAL(timeout()), this, SLOT(update()));
  m_updateTimer.setInterval(500);
}

Plotter::~Plotter()
{
  delete m_layout;
  delete s_rootWidget;
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

void Plotter::mousePressEvent(QMouseEvent* event)
{
  if (event->button() == Qt::MidButton)
    unzoom();
  QWidget::mousePressEvent(event);
}

void Plotter::mouseMoveEvent(QMouseEvent* event)
{
  if (m_selectedPlot < 0)
    return;
  double x = gPad->AbsPixeltoX(event->x());
  double y = gPad->AbsPixeltoY(event->y());
  emit(positionChanged(x, y));
  QWidget::mouseMoveEvent(event);
}

void Plotter::saveCanvas(const QString& fileName)
{
  s_rootWidget->GetCanvas()->SaveAs(qPrintable(fileName));
}

void Plotter::saveForPostAnalysis(const QString& fileName)
{
  int savedSelectedPlot = m_selectedPlot;
  TFile file(qPrintable(fileName), "RECREATE");
  for (unsigned int i = 0; i < numberOfPlots(); ++i) {
    selectPlot(i, true);
    s_rootWidget->GetCanvas()->SetName(qPrintable(plotTitle(i) + " canvas"));
    s_rootWidget->GetCanvas()->Write();
  }
  file.Close();
  selectPlot(savedSelectedPlot);
  s_rootWidget->GetCanvas()->SetName("tqtwidget");
}

void Plotter::update()
{
  if (0 <= m_selectedPlot && m_selectedPlot < m_plots.size())
    m_plots[m_selectedPlot]->update();
  gPad->Modified();
  gPad->Update();
}

void Plotter::updateCanvas()
{
  gPad->Modified();
  gPad->Update();
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

QVector<unsigned int> Plotter::plotIndices(AnalysisPlot::Topic topic)
{
  QVector<unsigned int> ret;
  for (int i = 0; i < m_plots.size(); ++i)
    if (m_plots[i]->topic() == topic)
      ret.append(i);
  return ret;
}

AnalysisPlot::Topic Plotter::plotTopic(unsigned int i)
{
  Q_ASSERT(i < numberOfPlots());
  return m_plots[i]->topic();
}

void Plotter::selectPlot(int i, bool inhibitDraw)
{
  Q_ASSERT(i < int(numberOfPlots()));
  if (i < 0) {
    emit(titleChanged(QString()));
    emit(positionChanged(0, 0));
    gPad->Clear();
  } else {
    emit(titleChanged(m_plots[i]->title()));
    m_plots[i]->draw(s_rootWidget->GetCanvas());
    if (!inhibitDraw) {
      if (m_layout->count() > 1) {
        m_layout->itemAt(0)->widget()->close();
      }
      QWidget* secondaryWidget = m_plots[i]->secondaryWidget();
      if (secondaryWidget) {
        m_layout->insertWidget(0, secondaryWidget);
        secondaryWidget->show();
      }
      updateCanvas();
    }
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
  updateCanvas();
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
  updateCanvas();
}

void Plotter::setGrid(bool b)
{
  gPad->SetGridx(b);
  gPad->SetGridy(b);
  updateCanvas();
}

void Plotter::setLogX(bool b)
{
  gPad->SetLogx(b);
  updateCanvas();
}

void Plotter::setLogY(bool b)
{
  gPad->SetLogy(b);
  updateCanvas();
}

void Plotter::setLogZ(bool b)
{
  gPad->SetLogz(b);
  updateCanvas();
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

TCanvas* Plotter::canvas()
{
  return s_rootWidget->GetCanvas();
}
