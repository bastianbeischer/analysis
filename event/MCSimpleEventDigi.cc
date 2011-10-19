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

MCSimpleEventDigi::MCSimpleEventDigi(const MCSimpleEventDigi& other)
  : TObject()
  , m_type(other.m_type)
  , m_detID(other.m_detID)
{
  m_signals.clear();

  const std::vector <const MCDigiSignal*> otherMcDigiSignals = other.digiSignals();
  for (unsigned int i = 0; i < otherMcDigiSignals.size(); i++) {
    const MCDigiSignal* otherMcDigiSignal = otherMcDigiSignals.at(i);
    m_signals.push_back(new MCDigiSignal( *otherMcDigiSignal ));
  }
}

MCSimpleEventDigi::~MCSimpleEventDigi()
{
  for (unsigned int i = 0; i < m_signals.size(); i++) {
    delete m_signals.at(i);
  }
}


double MCSimpleEventDigi::signalSum() const
{
  //sum up all signals (check if thy belong to one channel, atm only for trd true)
  if (m_signals.size() == 0)
    return 0.;

  double sum = 0.;
  std::vector<const MCDigiSignal*>::const_iterator it;
  for (it = m_signals.begin(); it != m_signals.end(); ++it)
    sum += (*it)->energyDeposition;

  return sum;
}

