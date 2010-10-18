#include "SimpleEvent.hh"

ClassImp( SimpleEvent );

SimpleEvent::SimpleEvent() :
  m_eventId(0),
  m_time(0)
{
}

SimpleEvent::SimpleEvent(int id, int time) :
  m_eventId(id),
  m_time(time)
{
}

SimpleEvent::~SimpleEvent()
{
}

