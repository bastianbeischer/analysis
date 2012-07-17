#include "SimpleEvent.hh"

#include "Hit.hh"
#include "Cluster.hh"
#include "DataDescription.hh"
#include "TOFSipmHit.hh"
#include "TOFCluster.hh"

#include <cmath>
#include <cassert>

#include <QDebug>

ClassImp(SimpleEvent);

SimpleEvent::SimpleEvent()
  : TObject()
  , m_eventId(0)
  , m_runStartTime(0)
  , m_eventTime(0)
  , m_contentType(None)
  , m_hits()
  , m_mcEventInformation(0)
  , m_description(0)
  , m_additionalHits()
{
  for (unsigned int i = 0; i < SensorTypes::N_SENSOR_TYPES; i++)
    m_sensorSet[i] = sqrt(-1);
}

SimpleEvent::SimpleEvent(unsigned int id, unsigned int runStartTime, unsigned int eventTime, ContentType type)
  : TObject()
  , m_eventId(id)
  , m_runStartTime(runStartTime)
  , m_eventTime(eventTime)
  , m_contentType(type)
  , m_hits()
  , m_mcEventInformation(0)
  , m_description(0)
  , m_additionalHits()
{
  for (unsigned int i = 0; i < SensorTypes::N_SENSOR_TYPES; i++)
    m_sensorSet[i] = sqrt(-1);
}

SimpleEvent::SimpleEvent(const SimpleEvent& other) :
  TObject(other),
  m_eventId(other.m_eventId),
  m_runStartTime(other.m_runStartTime),
  m_eventTime(other.m_eventTime),
  m_contentType(other.m_contentType),
  m_description(other.m_description)
{
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

  m_mcEventInformation = other.m_mcEventInformation ? new MCEventInformation(*other.m_mcEventInformation) : 0;
  for (unsigned int i = 0; i < SensorTypes::N_SENSOR_TYPES; i++) {
    m_sensorSet[i] = other.m_sensorSet[i];
  }

  std::vector<AdditionalHit*>::const_iterator it = other.m_additionalHits.begin();
  std::vector<AdditionalHit*>::const_iterator end = other.m_additionalHits.end();
  for (; it != end; ++it)
    m_additionalHits.push_back(new AdditionalHit(*(*it)));
}

SimpleEvent::~SimpleEvent()
{
  for (std::vector<Hit*>::iterator it = m_hits.begin(); it != m_hits.end(); it++) delete *it;
  for (std::vector<AdditionalHit*>::iterator it = m_additionalHits.begin(); it != m_additionalHits.end(); it++) delete *it;
  delete m_mcEventInformation;
}

void SimpleEvent::setSensorData(SensorTypes::Type type, float data)
{
  assert(type >= 0 && type < SensorTypes::N_SENSOR_TYPES);
  m_sensorSet[type] = data;
}

float SimpleEvent::sensorData(SensorTypes::Type type) const
{
  assert(type >= 0 && type < SensorTypes::N_SENSOR_TYPES);
  return m_sensorSet[type];
}

void SimpleEvent::setMCInformation(const MCEventInformation* mcInfo)
{
  if (m_mcEventInformation) delete m_mcEventInformation;
  m_mcEventInformation = mcInfo;
}

unsigned int SimpleEvent::eventNo() const
{
  assert(m_description);
  int run = m_description->runFileForEventId(m_eventId);
  int pedestalEvents = m_description->numberOfPedestalEventsInRunFile(run);
  int ledEvents = m_description->numberOfLedEventsInRunFile(run);
  return m_eventId - pedestalEvents - ledEvents;
}
