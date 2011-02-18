#ifndef MCSIMPLEEVENT_HH
#define MCSIMPLEEVENT_HH

#include "SimpleEvent.hh"

#include "MCEventInformation.hh"

class MCSimpleEvent : public SimpleEvent
{
public:
    MCSimpleEvent();
    MCSimpleEvent(const MCSimpleEvent& other);
    MCSimpleEvent(unsigned int id, unsigned int runStartTime, unsigned int eventTime, const MCEventInformation* mcInfo, ContentType type);
    ~MCSimpleEvent();

    const MCEventInformation* MCInformation() const   {return m_mcEventInformation;}
    void MCInformation(const MCEventInformation* mcInfo)
    {
      if (m_mcEventInformation) delete m_mcEventInformation;
      m_mcEventInformation = mcInfo;
    }

private:
    const MCEventInformation* m_mcEventInformation;


    ClassDef( MCSimpleEvent, 1 );

};

#endif // MCSIMPLEEVENT_HH
