#ifndef SimpleEvent_hh
#define SimpleEvent_hh

#include <TObject.h>

#include "Hit.hh"
#include "ECALHit.hh"
#include "MCEventInformation.hh"
#include "SensorTypes.hh"

#include <map>

class DataDescription;

class SimpleEvent :
  public TObject
{

public:
  enum ContentType {None = -1, Data, MonteCarlo};

public:
  SimpleEvent();
  SimpleEvent(unsigned int id, unsigned int runStartTime, unsigned int eventTime, ContentType type);
  SimpleEvent(const SimpleEvent& other);
  ~SimpleEvent();

public:
  const std::vector<Hit*>& hits() const {return m_hits;}
  unsigned int eventId() const {return m_eventId;}
  unsigned int eventNo() const;
  double time() const {return m_runStartTime + m_eventTime/1000.;}
  unsigned int runStartTime() const {return m_runStartTime;}
  unsigned int eventTime() const {return m_eventTime;}
  ContentType contentType() const {return m_contentType;}
  float sensorData(SensorTypes::Type type) const;
  const MCEventInformation* MCInformation() const {return m_mcEventInformation;}
  const DataDescription* description() const {return m_description;}

  void setEventId(unsigned int id) {m_eventId = id;}
  void setRunStartTime(unsigned int time) {m_runStartTime = time;}
  void setEventTime(unsigned int time) {m_eventTime = time;}
  void setContentType(ContentType type) {m_contentType = type;}
  void setSensorData(SensorTypes::Type type, float data);
  void setMCInformation(const MCEventInformation* mcInfo);
  void setDescription(const DataDescription* desc) {m_description = desc;}

  void addHit(Hit* hit) {m_hits.push_back(hit);}
  
  void addEcalHit(ECALHit*);
  const std::vector<ECALHit*>& ecalHits() const {return m_ecalHits;}

private:
  unsigned int m_eventId;
  unsigned int m_runStartTime; // in unix time (seconds)
  unsigned int m_eventTime; // in ms since the start of the run
  ContentType m_contentType;
  std::vector<Hit*> m_hits;
  std::vector<ECALHit*> m_ecalHits;
  float m_sensorSet[SensorTypes::N_SENSOR_TYPES];
  const MCEventInformation* m_mcEventInformation;
  const DataDescription* m_description; //!

  ClassDef( SimpleEvent, 1 );

};

#endif /* SimpleEvent_hh */
