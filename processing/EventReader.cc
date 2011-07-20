#include "EventReader.hh"

#include "SimpleEvent.hh"
#include "LimitedEventQueue.hh"
#include "DataChain.hh"
#include "ProcessingThread.hh"
#include "EventProcessor.hh"
#include "EventDestination.hh"
#include "Track.hh"
#include "Corrections.hh"

#include <QDebug>

EventReader::EventReader(QObject* parent)
  : QThread(parent)
  , m_abort(true)
  , m_firstEvent(0)
  , m_lastEvent(0)
  , m_nEvents(0)
  , m_readEvents(0)
  , m_chain(new DataChain())
  , m_nThreads(0)
  , m_bufferSize(1000)
  , m_threads()
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
  return 100.* queuedEvents() / (m_nThreads * m_bufferSize);
}

int EventReader::bufferSize() const
{
  return m_bufferSize;
}

void EventReader::setBufferSize(int size)
{
  m_bufferSize = size;
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

void EventReader::start(QVector<EventProcessor*>& processors)
{
  start(processors, 0, m_chain->nEntries() - 1);
}

void EventReader::start(QVector<EventProcessor*>& processors, unsigned int first, unsigned int last)
{
  m_nThreads = processors.size();;
  for (int i = 0; i < m_nThreads; ++i) {
    m_threads.append(new ProcessingThread(processors.at(i)));
    m_threads.at(i)->queue()->setBufferSize(m_bufferSize);
  }
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
  foreach(ProcessingThread* thread, m_threads)
    thread->start();

  Q_ASSERT(m_firstEvent <= m_lastEvent && m_lastEvent < m_chain->nEntries());
  unsigned int nEvents = m_lastEvent - m_firstEvent + 1;

  for (m_readEvents = 0; m_readEvents < nEvents;) {
    foreach(ProcessingThread* thread, m_threads) {
      if (thread->queue()->freeSpace() > 0 && m_readEvents < nEvents) {
        SimpleEvent* event = m_chain->event(m_firstEvent + m_readEvents);
        thread->queue()->enqueue(event);
        ++m_readEvents;
      }
    }

    if (m_abort)
      break;
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
}

QDateTime EventReader::startTime()
{
  return m_chain->startTime();
}

QDateTime EventReader::stopTime()
{
  return m_chain->stopTime();
}
