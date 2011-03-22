#include "MCEventInformation.hh"

ClassImp(MCEventInformation);

MCEventInformation::MCEventInformation()
  : TObject()
{
}

/* needed? as all structures behave correctly with the defaule copy constructors ?
MCEventInformation::MCEventInformation(const MCEventInformation& other)
  : m_pdgId(other.m_pdgId)
  , m_initialMomentum(other.m_initialMomentum)
{
  for (unsigned int i = 0; i < other.m_trajectory.size(); i++) {
    m_trajectory.push_back(other.m_trajectory.at(i));
  }
}
*/
