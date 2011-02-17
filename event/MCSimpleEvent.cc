#include "MCSimpleEvent.hh"

MCSimpleEvent::MCSimpleEvent():
    m_mcEventInformation(NULL)
{

}


MCSimpleEvent::MCSimpleEvent(const SimpleEvent& other):
      SimpleEvent(other)
  {
  }

MCSimpleEvent::MCSimpleEvent(const MCSimpleEvent& other):
      SimpleEvent(other)
  {
    MCEventInformation* mcInfo = new MCEventInformation();
    *mcInfo = *other.MCInformation();
  }
