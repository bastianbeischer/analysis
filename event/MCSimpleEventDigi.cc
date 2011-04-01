#include "MCSimpleEventDigi.hh"
#include "MCDigiSignal.hh"

ClassImp(MCSimpleEventDigi);

MCSimpleEventDigi::MCSimpleEventDigi(Hit::ModuleType type, unsigned short detID)
  : TObject()
  , m_type(type)
  , m_detID(detID)
{
}

MCSimpleEventDigi::~MCSimpleEventDigi()
{
  for (unsigned int i = 0; i < m_signals.size(); i++) {
    delete m_signals.at(i);
  }
}
