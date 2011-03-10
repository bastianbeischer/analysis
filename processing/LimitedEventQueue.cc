#include "LimitedEventQueue.hh"

LimitedEventQueue::LimitedEventQueue(int bufferSize) :
  EventQueue(),
  m_bufferSize(bufferSize)
{
}

LimitedEventQueue::~LimitedEventQueue()
{
}

int LimitedEventQueue::freeSpace()
{
  QMutexLocker locker(&m_mutex);
  return m_bufferSize - m_queue.size();
}
