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
}

MCEventInformation::MCEventInformation(const MCEventInformation& other)
{
  const MCSimpleEventParticle* otherPrimary = other.primary();
  m_primary = new MCSimpleEventParticle( *otherPrimary );

  const std::vector <const MCSimpleEventParticle*> otherSecondaries = other.secondaries();

  for (unsigned int i = 0; i < otherSecondaries.size(); i++) {
    const MCSimpleEventParticle* otherSecondary = otherSecondaries.at(i);
    m_secondaries.push_back(new MCSimpleEventParticle( *otherSecondary ));
  }
}

