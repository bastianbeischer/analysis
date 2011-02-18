#include "MCEventInformation.hh"

ClassImp(MCEventInformation);

MCEventInformation::MCEventInformation() :
  TObject()
{
}

MCEventInformation::MCEventInformation(const MCEventInformation& other) :
  m_pdgId(other.m_pdgId),
  m_initialMomentum(m_initialMomentum)
{
  for (unsigned int i = 0; i < other.m_trajectory.size(); i++) {
    m_trajectory.push_back(other.m_trajectory.at(i));
  }
}
