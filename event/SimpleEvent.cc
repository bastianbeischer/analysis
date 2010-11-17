#include "SimpleEvent.hh"

#include "Hit.hh"
#include "Cluster.hh"
#include "TOFSipmHit.hh"
#include "TOFCluster.hh"

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
  m_time(other.m_time)
{
  m_hits.clear();
  for (std::vector<Hit*>::const_iterator it = other.m_hits.begin(); it != other.m_hits.end(); it++) {
    Hit* hit = *it;
    if (strcmp(hit->ClassName(), "Hit") == 0)
      m_hits.push_back(new Hit(*hit));
    else if (strcmp(hit->ClassName(), "Cluster") == 0) {
      Cluster* cluster = static_cast<Cluster*>(hit);
      m_hits.push_back(new Cluster(*cluster));
    }
    else if (strcmp(hit->ClassName(), "TOFSipmHit") == 0) {
      TOFSipmHit* tofHit = static_cast<TOFSipmHit*>(hit);
      m_hits.push_back(new TOFSipmHit(*tofHit));
    }
    else if (strcmp(hit->ClassName(), "TOFCluster") == 0) {
      TOFCluster* cluster = static_cast<TOFCluster*>(hit);
      m_hits.push_back(new TOFCluster(*cluster));
    }
  }
}

SimpleEvent::~SimpleEvent()
{
  for (std::vector<Hit*>::iterator it = m_hits.begin(); it != m_hits.end(); it++) {
    delete *it;
  }
}

