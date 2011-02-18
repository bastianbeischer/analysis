#include "MCSimpleEvent.hh"

MCSimpleEvent::MCSimpleEvent():
    m_mcEventInformation(NULL)
{
}

MCSimpleEvent::MCSimpleEvent(const MCSimpleEvent& other):
    SimpleEvent(other),
    m_mcEventInformation(new MCEventInformation(other))
{
}

MCSimpleEvent::MCSimpleEvent(unsigned int id, unsigned int runStartTime, unsigned int eventTime, const MCEventInformation* mcInfo, ContentType type):
    SimpleEvent(id, runStartTime, eventTime, type),
    m_mcEventInformation(new MCEventInformation(mcInfo))
{
}

MCSimpleEvent::~MCSimpleEvent()
{
  delete m_mcEventInformation;
}
