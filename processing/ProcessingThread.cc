#include "ProcessingThread.hh"

#include "EventDestination.hh"
#include "LimitedEventQueue.hh"
#include "EventProcessor.hh"
#include "SimpleEvent.hh"

ProcessingThread::ProcessingThread(EventProcessor* processor, QObject* parent) :
  QThread(parent),
  m_queue(new LimitedEventQueue),
  m_processor(processor),
  m_abort(true),
  m_mutex()
{
}

ProcessingThread::~ProcessingThread()
{
  delete m_queue;
}

void ProcessingThread::start()
{
  m_abort = false;
  QThread::start();
}

void ProcessingThread::stop()
{
  m_mutex.lock();
  m_abort = true;
  m_mutex.unlock();
  wait();
}

LimitedEventQueue* ProcessingThread::queue()
{
  QMutexLocker locker(&m_mutex);
  return m_queue;
}

void ProcessingThread::run()
{
  forever {
    SimpleEvent* event = m_queue->dequeue();
    if (event) {
      m_processor->process(event);
      delete event;
    } else {
      usleep(1000);
    }
    m_mutex.lock();
    if (m_abort) {
      m_mutex.unlock();
      return;
    }
    m_mutex.unlock();
  }
}
