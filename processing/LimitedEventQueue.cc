#include "LimitedEventQueue.hh"

LimitedEventQueue::LimitedEventQueue(int bufferSize) :
  EventQueue(),
  m_bufferSize(0)
{
  setBufferSize(bufferSize);
}

LimitedEventQueue::~LimitedEventQueue()
{
}

int LimitedEventQueue::freeSpace()
{
  QMutexLocker locker(&m_mutex);
  return m_bufferSize - m_queue.size();
}

void LimitedEventQueue::setBufferSize(int size)
{
  m_bufferSize = size;
  m_queue.reserve(m_bufferSize);
}
