#include "EventReader.hh"

#include "SimpleEvent.hh"
#include "EventQueue.hh"
#include "DataChain.hh"
#include "EventProcessor.hh"
#include "Corrections.hh"

EventReader::EventReader(DataChain* chain, QObject* parent)
  : QThread(parent)
  , m_mutex()
  , m_abort(true)
  , m_firstEvent(0)
  , m_lastEvent(0)
  , m_progress(0)
  , m_buffer(0)
  , m_chain(chain)
  , m_processors()
{}

EventReader::~EventReader()
{
  foreach(EventProcessor* processor, m_processors)
    delete processor;
}

void EventReader::addEventProcessor(EventProcessor* processor)
{
  Q_ASSERT(m_abort);
  m_processors.append(processor);
}

void EventReader::start(unsigned int first, unsigned int last)
{
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
  foreach(EventProcessor* processor, m_processors)
    processor->start();
  Q_ASSERT(m_firstEvent <= m_lastEvent && m_lastEvent < m_chain->nEntries());
  unsigned int nEvents = m_lastEvent - m_firstEvent + 1;

  int freeSpace = 0;
  int queuedEvents = 0;
  unsigned int i = 0;
  for (i = 0; i < nEvents;) {
    queuedEvents = 0;
    foreach(EventProcessor* processor, m_processors)
      queuedEvents+= processor->queue()->numberOfEvents();
    foreach(EventProcessor* processor, m_processors) {
      freeSpace = processor->queue()->freeSpace();
      if (freeSpace > .2 * EventQueue::s_bufferSize) {
        for (int j = 0; j < freeSpace && i < nEvents; ++j) {
          SimpleEvent* event = m_chain->event(m_firstEvent + i);
          processor->queue()->enqueue(new SimpleEvent(*event));
          ++i;
        }
      }
    }
    m_mutex.lock();
    if (m_abort)
      break;
    m_mutex.unlock();
  }

  do {
    queuedEvents = 0;
    foreach(EventProcessor* processor, m_processors)
      queuedEvents+= processor->queue()->numberOfEvents();
  } while (queuedEvents);

  foreach (EventProcessor* processor, m_processors)
    processor->stop();
  m_mutex.lock();
  m_abort = true;
  m_mutex.unlock();
  emit(eventLoopStopped());
}
