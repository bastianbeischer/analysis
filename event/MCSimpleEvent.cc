#include "MCSimpleEvent.hh"

MCSimpleEvent::MCSimpleEvent():
  m_mcEventInformation(0)
{
}

MCSimpleEvent::MCSimpleEvent(const MCSimpleEvent& other):
  SimpleEvent(other),
  m_mcEventInformation(new MCEventInformation(*other.m_mcEventInformation))
{
}

MCSimpleEvent::MCSimpleEvent(unsigned int id, unsigned int runStartTime, unsigned int eventTime, ContentType type, const MCEventInformation* mcInfo):
  SimpleEvent(id, runStartTime, eventTime, type),
  m_mcEventInformation(new MCEventInformation(*mcInfo))
{
}

MCSimpleEvent::~MCSimpleEvent()
{
  delete m_mcEventInformation;
}

void MCSimpleEvent::setMCInformation(const MCEventInformation* mcInfo)
{
  if (m_mcEventInformation) delete m_mcEventInformation;
  m_mcEventInformation = mcInfo;
}
