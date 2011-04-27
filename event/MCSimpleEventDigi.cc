#include "MCSimpleEventDigi.hh"
#include "MCDigiSignal.hh"

ClassImp(MCSimpleEventDigi);


MCSimpleEventDigi::MCSimpleEventDigi()
  : TObject()
{
}

MCSimpleEventDigi::MCSimpleEventDigi(Hit::ModuleType type, unsigned short detID)
  : TObject()
  , m_type(type)
  , m_detID(detID)
{
}

MCSimpleEventDigi::MCSimpleEventDigi(const MCSimpleEventDigi& other) :
  m_type(other.m_type),
  m_detID(other.m_detID)
{
  m_signals.clear();
  for (std::vector<const MCDigiSignal*>::iterator it = m_signals.begin(); it != m_signals.end(); it++) {
    const MCDigiSignal* otherDigiSignal = *it;
    m_signals.push_back(new MCDigiSignal(*otherDigiSignal));
  }
}

MCSimpleEventDigi::~MCSimpleEventDigi()
{
  for (unsigned int i = 0; i < m_signals.size(); i++) {
    delete m_signals.at(i);
  }
}
