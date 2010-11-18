#include "Plotter.hh"
#include "AnalysisPlot.hh"
#include "SimpleEvent.hh"
#include "Setup.hh"
#include "DataChain.hh"
#include "EventQueue.hh"
#include "AnalysisThread.hh"

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
  , m_dataChainProgressBar(0)
  , m_eventQueueProgressBar(0)
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

void Plotter::setDataChainProgressBar(QProgressBar* bar)
{
  m_dataChainProgressBar = bar;
}

void Plotter::setEventQueueProgressBar(QProgressBar* bar)
{
  m_eventQueueProgressBar = bar;
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

void Plotter::finalizeAnalysis()
{
  foreach(AnalysisPlot* plot, m_plots)
    plot->finalize();
}

void Plotter::abortAnalysis()
{
  m_eventLoopOff = true;
}

void Plotter::startAnalysis(Track::Type type, int numberOfThreads)
{
  m_eventLoopOff = false;

  EventQueue queue;
  QVector<AnalysisThread*> threads;
  for (int i = 0; i < numberOfThreads; ++i) {
    AnalysisThread* thread = new AnalysisThread(&queue, type, m_plots, this);
    threads.append(thread);
    thread->start();
  }

  if (m_dataChainProgressBar)
    m_dataChainProgressBar->reset();
  unsigned int nEntries = m_chain->nEntries();
  int freeSpace = 0;
  int queuedEvents = 0;
  for (unsigned int i = 0; i < nEntries;) {
    freeSpace = queue.freeSpace();
    queuedEvents = queue.numberOfEvents();
    if (freeSpace > .2 * EventQueue::s_bufferSize) {
      for (int j = 0; j < freeSpace && i < nEntries; ++j) {
        SimpleEvent* event = m_chain->event(i);
        queue.enqueue(new SimpleEvent(*event));
        ++i;
        if (m_dataChainProgressBar)
          m_dataChainProgressBar->setValue(100 * (i + 1) / nEntries);
        if (m_eventQueueProgressBar)
          m_eventQueueProgressBar->setValue(100 * queuedEvents / EventQueue::s_bufferSize);
        qApp->processEvents();
      }
    }
    if (m_eventLoopOff)
        break;
    qApp->processEvents();
  }

  do {
    queuedEvents = queue.numberOfEvents();
    m_eventQueueProgressBar->setValue(100 * queuedEvents / EventQueue::s_bufferSize);
    qApp->processEvents();
  } while (queuedEvents);
  foreach (AnalysisThread* thread, threads)
    thread->stop();
  qDeleteAll(threads);
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
