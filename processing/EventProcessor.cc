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
