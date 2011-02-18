#ifndef MCSIMPLEEVENT_HH
#define MCSIMPLEEVENT_HH

#include "SimpleEvent.hh"

#include "MCEventInformation.hh"

class MCSimpleEvent : public SimpleEvent
{
public:
  MCSimpleEvent();
  MCSimpleEvent(const MCSimpleEvent& other);
  MCSimpleEvent(unsigned int id, unsigned int runStartTime, unsigned int eventTime, ContentType type, const MCEventInformation* mcInfo);
  ~MCSimpleEvent();

  const MCEventInformation* MCInformation() const {return m_mcEventInformation;}
  void setMCInformation(const MCEventInformation* mcInfo);

private:
  const MCEventInformation* m_mcEventInformation;

  ClassDef(MCSimpleEvent, 1);

};

#endif // MCSIMPLEEVENT_HH
