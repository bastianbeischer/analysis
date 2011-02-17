#ifndef SimpleEvent_hh
#define SimpleEvent_hh

#include <TObject.h>

#include "Hit.hh"
#include "SensorTypes.hh"

#include <map>

class SimpleEvent :
  public TObject
{

public:
  enum ContentType {None = -1, RawData, Clusters};

public:
  SimpleEvent();
  SimpleEvent(unsigned int id, unsigned int runStartTime, unsigned int eventTime, ContentType type);
  SimpleEvent(const SimpleEvent& other);
  ~SimpleEvent();

public:
  std::vector<Hit*>& hits()               {return m_hits;}
  unsigned int       eventId() const      {return m_eventId;}
  double             time()    const      {return m_runStartTime + m_eventTime/1000.;}
  unsigned int       runStartTime() const {return m_runStartTime;}
  unsigned int       eventTime() const    {return m_eventTime;}
  ContentType        contentType() const  {return m_contentType;}

  void               addHit(Hit* hit) {m_hits.push_back(hit);}

  void setSensorData(SensorTypes::Type type, float data);
  float sensorData(SensorTypes::Type type);

private:
  unsigned int      m_eventId;
  unsigned int      m_runStartTime; // in unix time (seconds)
  unsigned int      m_eventTime; // in ms since the start of the run
  ContentType       m_contentType;
  std::vector<Hit*> m_hits;
  float             m_sensorSet[SensorTypes::N_SENSOR_TYPES]; //!

  ClassDef( SimpleEvent, 1 );
  
};

#endif /* SimpleEvent_hh */
