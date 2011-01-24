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
#include <TFile.h>

#include <iostream>

Plotter::Plotter(QWidget* parent)
  : TQtWidget(parent)
  , m_titleLabel(0)
  , m_positionLabel(0)
  , m_timeLabel(0)
  , m_time()
  , m_updateTimer(this)
  , m_dataChainProgressBar(0)
  , m_eventQueueProgressBar(0)
  , m_chain(new DataChain())
  , m_eventLoopOff(true)
  , m_selectedPlot(-1)
{
  gROOT->cd();
  setMouseTracking(true);
  connect(&m_updateTimer, SIGNAL(timeout()), this, SLOT(update()));
  m_updateTimer.start(500);
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

void Plotter::saveForPostAnalysis(const QString& fileName)
{
  int savedSelectedPlot = m_selectedPlot;
  TFile file(qPrintable(fileName), "RECREATE");
  for (unsigned int i = 0; i < numberOfPlots(); ++i) {
    selectPlot(i);
    GetCanvas()->SetName(qPrintable(plotTitle(i) + " canvas"));
    GetCanvas()->Write();
  }
  file.Close();
  selectPlot(savedSelectedPlot);
  GetCanvas()->SetName("tqtwidget");
}

void Plotter::update()
{
  if (!m_eventLoopOff && m_timeLabel)
    m_timeLabel->setText(QString("%1s").arg(m_time.elapsed()/1000));
  if (0 <= m_selectedPlot && m_selectedPlot < m_plots.size())
    m_plots[m_selectedPlot]->update();
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

void Plotter::startAnalysis(Track::Type type, Corrections::Flags flags, int numberOfThreads)
{
  m_eventLoopOff = false;

  QVector<EventQueue*> queues;
  QVector<AnalysisThread*> threads;
  for (int i = 0; i < numberOfThreads; ++i) {
    EventQueue* queue = new EventQueue();
    queues.append(queue);
    AnalysisThread* thread = new AnalysisThread(queue, type, flags, m_plots, this);
    threads.append(thread);
    thread->start();
  }

  m_time.restart();
  if (m_dataChainProgressBar)
    m_dataChainProgressBar->reset();

  unsigned int nEntries = m_chain->nEntries();
  int freeSpace = 0;
  int queuedEvents = 0;
  for (unsigned int i = 0; i < nEntries;) {
    queuedEvents = 0;
    foreach(EventQueue* queue, queues)
      queuedEvents+= queue->numberOfEvents();
    foreach(EventQueue* queue, queues) {
      freeSpace = queue->freeSpace();
      if (freeSpace > .2 * EventQueue::s_bufferSize) {
        for (int j = 0; j < freeSpace && i < nEntries; ++j) {
          SimpleEvent* event = m_chain->event(i);
          queue->enqueue(new SimpleEvent(*event));
          ++i;
          if (m_dataChainProgressBar)
            m_dataChainProgressBar->setValue(100 * (i + 1) / nEntries);
          if (m_eventQueueProgressBar)
            m_eventQueueProgressBar->setValue(100 * queuedEvents / EventQueue::s_bufferSize);
          qApp->processEvents();
        }
      }
    }
    if (m_eventLoopOff)
        break;
    qApp->processEvents();
  }

  do {
    queuedEvents = 0;
    foreach(EventQueue* queue, queues)
      queuedEvents+= queue->numberOfEvents();
    m_eventQueueProgressBar->setValue(100 * queuedEvents / EventQueue::s_bufferSize);
    qApp->processEvents();
  } while (queuedEvents);
  foreach (AnalysisThread* thread, threads)
    thread->stop();
  qDeleteAll(threads);
  qDeleteAll(queues);
  finalizeAnalysis();
  m_eventLoopOff = true;
}

void Plotter::addFileList(const QString& fileName)
{
  m_chain->addFileList(qPrintable(fileName));
}

void Plotter::setFileList(const QString& fileName)
{
  m_chain->setFileList(qPrintable(fileName));
}

void Plotter::setTimeLabel(QLabel* label)
{
  m_timeLabel = label;
}
