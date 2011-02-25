#include "EventReader.hh"

#include "SimpleEvent.hh"
#include "EventQueue.hh"
#include "DataChain.hh"
#include "ProcessingThread.hh"
#include "AnalysisProcessor.hh"
#include "EventDestination.hh"
#include "Track.hh"
#include "Corrections.hh"

#include <QDebug>

EventReader::EventReader(QObject* parent)
  : QThread(parent)
  , m_mutex()
  , m_abort(true)
  , m_trackType(Track::None)
  , m_correctionFlags(Corrections::None)
  , m_nThreads(0)
  , m_firstEvent(0)
  , m_lastEvent(0)
  , m_nEvents(0)
  , m_chain(new DataChain())
  , m_destinations()
{
}

EventReader::~EventReader()
{
  delete m_chain;
}

int EventReader::queuedEvents() const
{
  int queuedEvents = 0;
  foreach(ProcessingThread* thread, m_threads)
    queuedEvents += thread->queue()->numberOfEvents();
  return queuedEvents;
}

double EventReader::progress() const
{
  return 100. * (m_readEvents - queuedEvents()) / m_nEvents;
}

double EventReader::buffer() const
{
  return 100.* queuedEvents() / (m_nThreads * EventQueue::s_bufferSize);
}

void EventReader::addDestination(EventDestination* destination)
{
  Q_ASSERT(m_abort);
  m_destinations.append(destination);
}

void EventReader::clearDestinations()
{
  m_destinations.clear();
}
void EventReader::setFileList(const QString& fileName)
{
  m_chain->setFileList(qPrintable(fileName));
  emit(numberOfEventsChanged(m_chain->nEntries()));
}

void EventReader::addFileList(const QString& fileName)
{
  m_chain->addFileList(qPrintable(fileName));
  emit(numberOfEventsChanged(m_chain->nEntries()));
}

void EventReader::addRootFile(const QString& file)
{
  m_chain->addRootFile(qPrintable(file));
  emit(numberOfEventsChanged(m_chain->nEntries()));
}

void EventReader::start(Track::Type trackType, Corrections::Flags flags, int nThreads)
{
  start(trackType, flags, nThreads, 0, m_chain->nEntries() - 1);
}

void EventReader::start(Track::Type trackType, Corrections::Flags flags, int nThreads, unsigned int first, unsigned int last)
{
  m_trackType = trackType;
  m_correctionFlags = flags;
  m_nThreads = nThreads;
  m_firstEvent = first;
  m_lastEvent = last;
  m_nEvents = last - first + 1;
  m_abort = false;
  QThread::start();
}

void EventReader::stop()
{
  m_mutex.lock();
  m_abort = true;
  m_mutex.unlock();
  wait();
}

void EventReader::run()
{
  emit(eventLoopStarted());
  for (int thread = 0; thread < m_nThreads; ++thread) {
    AnalysisProcessor* processor = new AnalysisProcessor(m_destinations, m_trackType, m_correctionFlags);
    ProcessingThread* thread = new ProcessingThread(processor);
    m_threads.append(thread);
    thread->start();
  }

  Q_ASSERT(m_firstEvent <= m_lastEvent && m_lastEvent < m_chain->nEntries());
  unsigned int nEvents = m_lastEvent - m_firstEvent + 1;

  for (m_readEvents = 0; m_readEvents < nEvents;) {
    foreach(ProcessingThread* thread, m_threads) {
      if (thread->queue()->freeSpace() > 0) {
        SimpleEvent* event = m_chain->event(m_firstEvent + m_readEvents);
        thread->queue()->enqueue(new SimpleEvent(*event));
        m_mutex.lock();
        ++m_readEvents;
        m_mutex.unlock();
      }
    }

    m_mutex.lock();
    if (m_abort) {
      m_mutex.unlock();
      break;
    }
    m_mutex.unlock();
  }
  
  do {
    usleep(10000);
  } while (queuedEvents());

  foreach (ProcessingThread* thread, m_threads)
    thread->stop();

  m_mutex.lock();
  m_abort = true;
  m_mutex.unlock();

  qDeleteAll(m_threads);
  m_threads.clear();

  emit(eventLoopStopped());
}
