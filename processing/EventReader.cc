#include "EventReader.hh"

#include "SimpleEvent.hh"
#include "EventQueue.hh"
#include "DataChain.hh"
#include "EventProcessor.hh"
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
  , m_progress(0)
  , m_buffer(0)
  , m_chain(new DataChain())
  , m_destinations()
{
}

EventReader::~EventReader()
{
  delete m_chain;
}

int EventReader::progress() const
{
  return m_progress;
}

int EventReader::buffer() const
{
  return m_buffer;
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
  QVector<EventProcessor*> processors;
  for (int thread = 0; thread < m_nThreads; ++thread) {
    EventProcessor* processor = new EventProcessor(m_trackType, m_correctionFlags, m_destinations);
    processors.append(processor);
    processor->start();
  }

  Q_ASSERT(m_firstEvent <= m_lastEvent && m_lastEvent < m_chain->nEntries());
  unsigned int nEvents = m_lastEvent - m_firstEvent + 1;

  int freeSpace = 0;
  int queuedEvents = 0;
  unsigned int i = 0;
  for (i = 0; i < nEvents;) {
    queuedEvents = 0;
    foreach(EventProcessor* processor, processors)
      queuedEvents+= processor->queue()->numberOfEvents();
    foreach(EventProcessor* processor, processors) {
      freeSpace = processor->queue()->freeSpace();
      if (freeSpace > .2 * EventQueue::s_bufferSize) {
        for (int j = 0; j < freeSpace && i < nEvents; ++j) {
          SimpleEvent* event = m_chain->event(m_firstEvent + i);
          processor->queue()->enqueue(new SimpleEvent(*event));
          ++i;
          ++queuedEvents;
          m_progress = 100. * (i + 1) / nEvents;
          m_buffer = 100. * queuedEvents / (m_nThreads * EventQueue::s_bufferSize);
        }
      }
      m_buffer = 100. * queuedEvents / (m_nThreads * EventQueue::s_bufferSize);
    }
    m_mutex.lock();
    if (m_abort) {
      m_mutex.unlock();
      break;
    }
    m_mutex.unlock();
  }

  do {
    queuedEvents = 0;
    foreach(EventProcessor* processor, processors)
      queuedEvents+= processor->queue()->numberOfEvents();
    m_buffer = 100. * queuedEvents / (m_nThreads * EventQueue::s_bufferSize);
  } while (queuedEvents);

  foreach (EventProcessor* processor, processors)
    processor->stop();
  qDeleteAll(processors);
  
  m_mutex.lock();
  m_abort = true;
  m_mutex.unlock();

  emit(eventLoopStopped());
}
