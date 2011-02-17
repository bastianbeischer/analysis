#include "MCSimpleEvent.hh"

MCSimpleEvent::MCSimpleEvent():
    m_mcEventInformation(NULL)
{

}


MCSimpleEvent::MCSimpleEvent(const MCSimpleEvent& other):
    SimpleEvent(other)
{
  MCEventInformation* mcInfo = new MCEventInformation();
  *mcInfo = *other.MCInformation();
}


MCSimpleEvent::~MCSimpleEvent()
{
  delete m_mcEventInformation;
}
