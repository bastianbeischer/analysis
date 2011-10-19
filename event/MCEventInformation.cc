#include "MCEventInformation.hh"


ClassImp(MCEventInformation);

MCEventInformation::MCEventInformation()
  : TObject()
  , m_primary(0)
{
}

MCEventInformation::~MCEventInformation()
{
  if(m_primary)
    delete m_primary;

  for (unsigned int i = 0; i < m_secondaries.size(); i++) {
    delete m_secondaries.at(i);
  }

  for (unsigned int i = 0; i < m_mcDigis.size(); i++) {
    delete m_mcDigis.at(i);
  }
}

MCEventInformation::MCEventInformation(const MCEventInformation& other)
  : TObject()
{
  const MCSimpleEventParticle* otherPrimary = other.primary();
  m_primary = new MCSimpleEventParticle( *otherPrimary );

  const std::vector <const MCSimpleEventParticle*> otherSecondaries = other.secondaries();
  for (unsigned int i = 0; i < otherSecondaries.size(); i++) {
    const MCSimpleEventParticle* otherSecondary = otherSecondaries.at(i);
    m_secondaries.push_back(new MCSimpleEventParticle( *otherSecondary ));
  }

  const std::vector <const MCSimpleEventDigi*> otherMcDigis = other.mcDigis();
  for (unsigned int i = 0; i < otherMcDigis.size(); i++) {
    const MCSimpleEventDigi* otherMcDigi = otherMcDigis.at(i);
    m_mcDigis.push_back(new MCSimpleEventDigi( *otherMcDigi ));
  }
}

