#include "EventProcessor.hh"

#include "EventDestination.hh"

EventProcessor::EventProcessor()
{
}

EventProcessor::EventProcessor(QVector<EventDestination*> destinations)
  : m_destinations(destinations)
{
}

EventProcessor::~EventProcessor()
{
}

void EventProcessor::addDestination(EventDestination* destination)
{
  m_destinations.append(destination);
}

void EventProcessor::clearDestinations()
{
  m_destinations.clear();
}

bool EventProcessor::tryProcessingDestination(EventDestination* destination, QVector<Hit*>& clusters, Particle* particle, SimpleEvent* event) const
{
  if (destination->needsLocking()) { // destination needs locking
    if (destination->tryLock()) { // if tryLock() yields "true" destination has been locked successfully and was not locked before
      destination->processEvent(clusters, particle, event);
      destination->unlock();
      return true;
    }
    else { // can't process right now
      return false;
    }
  }
  else { // destination does not need locking
    destination->processEvent(clusters, particle, event);
    return true;
  }
}
