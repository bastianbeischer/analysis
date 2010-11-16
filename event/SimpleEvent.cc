#include "SimpleEvent.hh"

ClassImp( SimpleEvent );

SimpleEvent::SimpleEvent() :
  TObject(),
  m_eventId(0),
  m_time(0),
  m_hits()
{
}

SimpleEvent::SimpleEvent(int id, int time) :
  TObject(),
  m_eventId(id),
  m_time(time),
  m_hits()
{
}

SimpleEvent::SimpleEvent(const SimpleEvent& other) :
  TObject(other),
  m_eventId(other.m_eventId),
  m_time(other.m_time),
  m_hits(other.m_hits)
{
}

const SimpleEvent& SimpleEvent::operator=(const SimpleEvent& right)
{
  static_cast<TObject>(*this) = static_cast<TObject>(right);
  m_eventId = right.m_eventId;
  m_time = right.m_time;
  m_hits = right.m_hits;
  return *this;
}

SimpleEvent::~SimpleEvent()
{
  for (std::vector<Hit*>::iterator it = m_hits.begin(); it != m_hits.end(); it++) {
    delete *it;
  }
}

