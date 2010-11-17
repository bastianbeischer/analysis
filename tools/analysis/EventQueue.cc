#include "EventQueue.hh"

#include "SimpleEvent.hh"
#include <iostream>
EventQueue::EventQueue()
  : m_queue()
{}

EventQueue::~EventQueue()
{
  foreach(SimpleEvent* event, m_queue)
    delete event;
}

int EventQueue::freeSpace()
{
  QMutexLocker locker(&m_mutex);
  return s_bufferSize - m_queue.size();
}

int EventQueue::numberOfEvents()
{
  QMutexLocker locker(&m_mutex);
  return m_queue.size();
}

void EventQueue::enqueue(SimpleEvent* event)
{
  QMutexLocker locker(&m_mutex);
  m_queue.enqueue(event);
  std::cout << '+' << std::flush;
}

SimpleEvent* EventQueue::dequeue()
{
  QMutexLocker locker(&m_mutex);
  if (m_queue.isEmpty())
    return 0;
  std::cout << '-' << std::flush;
  return m_queue.dequeue();
}
