#ifndef MCSIMPLEEVENT_HH
#define MCSIMPLEEVENT_HH

#include "SimpleEvent.hh"

#include "MCEventInformation.hh"

class MCSimpleEvent : public SimpleEvent
{
public:
    MCSimpleEvent();

    MCEventInformation* MCInformation() const         {return m_mcEventInformation;}
    void MCInformation(MCEventInformation* mcInfo)    {m_mcEventInformation = mcInfo;}

private:
    MCEventInformation* m_mcEventInformation;


    ClassDef( MCSimpleEvent, 1 );

};

#endif // MCSIMPLEEVENT_HH
