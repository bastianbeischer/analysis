#include "Event.hh"

ClassImp( Event );

Event::Event() :
  m_eventId(0),
  m_time(0)
{
}

Event::Event(int id, int time) :
  m_eventId(id),
  m_time(time)
{
}

Event::~Event()
{
}

