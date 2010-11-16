#include "Plotter.hh"
#include "AnalysisPlot.hh"
#include "TrackFinding.hh"
#include "SimpleEvent.hh"
#include "Hit.hh"
#include "Cluster.hh"
#include "Track.hh"
#include "Setup.hh"
#include "DataChain.hh"

#include <QApplication>
#include <QLabel>
#include <QProgressBar>
#include <QDebug>

#include <TPad.h>
#include <TROOT.h>

#include <iostream>

Plotter::Plotter(QWidget* parent)
  : TQtWidget(parent)
  , m_titleLabel(0)
  , m_positionLabel(0)
  , m_progressBar(0)
  , m_track(0)
  , m_trackFinding(new TrackFinding())
  , m_chain(new DataChain())
  , m_eventLoopOff(true)
  , m_selectedPlot(-1)
{
  gROOT->cd();
  setMouseTracking(true);
}

Plotter::~Plotter()
{
  qDeleteAll(m_plots);
  delete m_trackFinding;
  if (m_track)
    delete m_track;
  delete m_chain;
}

unsigned int Plotter::numberOfPlots()
{
  return m_plots.size();
}

void Plotter::setTitleLabel(QLabel* label)
{
  m_titleLabel = label;
}

void Plotter::setPositionLabel(QLabel* label)
{
  m_positionLabel = label;
}

void Plotter::setProgressBar(QProgressBar* bar)
{
  m_progressBar = bar;
}

void Plotter::mousePressEvent(QMouseEvent* event)
{
  TQtWidget::mousePressEvent(event);
}

void Plotter::mouseReleaseEvent(QMouseEvent* event)
{
  TQtWidget::mouseReleaseEvent(event);
}

void Plotter::mouseMoveEvent(QMouseEvent* event)
{
  if (!m_positionLabel || m_selectedPlot < 0)
    return;
  double x = gPad->AbsPixeltoX(event->x());
  double y = gPad->AbsPixeltoY(event->y());
  m_positionLabel->setText(QString("%1%2  %3%4")
    .arg(x < 0 ? '-' : '+').arg(qAbs(x), 7, 'f', 3, '0')
    .arg(y < 0 ? '-' : '+').arg(qAbs(y), 7, 'f', 3, '0'));
  TQtWidget::mouseMoveEvent(event);
}

void Plotter::saveCanvas(const QString& fileName)
{
  GetCanvas()->SaveAs(qPrintable(fileName));
}

void Plotter::update()
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

void Plotter::selectPlot(int i)
{
  Q_ASSERT(i < int(numberOfPlots()));
  if (i < 0) {
    if (m_titleLabel)
      m_titleLabel->clear();
    if (m_positionLabel)
      m_positionLabel->clear();
    gPad->Clear();
  } else {
    if (m_titleLabel)
      m_titleLabel->setText(m_plots[i]->title());
    m_plots[i]->draw(GetCanvas());
  }
  m_selectedPlot = i;
}

void Plotter::clearPlots()
{
  qDeleteAll(m_plots);
  m_plots.clear();
}

void Plotter::processEvent(SimpleEvent* event)
{
  QVector<Hit*> hits = QVector<Hit*>::fromStdVector(event->hits());
  QVector<Hit*> clusters;
  foreach(Cluster* cluster, Setup::instance()->generateClusters(hits))
    clusters.append(cluster);
  QVector<Hit*> trackClusters = m_trackFinding->findTrack(clusters);
  if (m_track)
    m_track->fit(trackClusters);
  foreach(AnalysisPlot* plot, m_plots)
    plot->processEvent(trackClusters, m_track, event);
  Setup::instance()->deleteClusters();
}

void Plotter::finalizeAnalysis()
{
  foreach(AnalysisPlot* plot, m_plots)
    plot->finalize();
}

void Plotter::setTrackType(Track* track)
{
  if (m_track)
    delete m_track;
  m_track = track;
}

void Plotter::abortAnalysis()
{
  m_eventLoopOff = true;
}

void Plotter::startAnalysis()
{
  m_eventLoopOff = false;
  if (m_progressBar)
    m_progressBar->reset();
  unsigned int nEntries = m_chain->nEntries();
  for (unsigned int i = 0; i < nEntries; i++) {
    SimpleEvent* event = m_chain->event(i);
    processEvent(event);
    if (m_progressBar)
       m_progressBar->setValue(100 * (i + 1) / nEntries);
    if (m_eventLoopOff)
      break;
    qApp->processEvents();
  }
  finalizeAnalysis();
}

void Plotter::addFileList(const QString& fileName)
{
  m_chain->addFileList(qPrintable(fileName));
}

void Plotter::setFileList(const QString& fileName)
{
  m_chain->setFileList(qPrintable(fileName));
}
