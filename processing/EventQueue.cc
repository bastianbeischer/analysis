#include "EventQueue.hh"

#include "SimpleEvent.hh"

EventQueue::EventQueue()
  : m_queue()
{}

EventQueue::~EventQueue()
{
  foreach(SimpleEvent* event, m_queue)
    delete event;
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
}

SimpleEvent* EventQueue::dequeue()
{
  QMutexLocker locker(&m_mutex);
  if (m_queue.isEmpty())
    return 0;
  return m_queue.dequeue();
}
