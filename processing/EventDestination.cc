#include "EventDestination.hh"
  
EventDestination::EventDestination() :
  m_needsLocking(false)
{
}

EventDestination::~EventDestination()
{
}

bool EventDestination::tryLock()
{
  return m_mutex.tryLock();
}

void EventDestination::lock()
{
  m_mutex.lock();
}

void EventDestination::unlock()
{
  m_mutex.unlock();
}
