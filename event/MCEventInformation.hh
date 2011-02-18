#ifndef MCEVENTINFORMATION_HH
#define MCEVENTINFORMATION_HH

#include <TObject.h>
#include <TVector3.h>
#include <vector>

class MCEventInformation  :
  public TObject
{
public:
  MCEventInformation();
  MCEventInformation(const MCEventInformation& other);

  int pdgId() const {return m_pdgId;}
  void setPdgId(int pdgId) {m_pdgId = pdgId;}

  TVector3 initialMomentum() const {return m_initialMomentum;}
  void setInitialMomentum(const TVector3& initialMomentum) {m_initialMomentum = initialMomentum;}

  std::vector<TVector3> trajectory() const {return m_trajectory;}
  void setTrajectory(const std::vector<TVector3> traj) {m_trajectory = traj;}

private:
  int m_pdgId;
  TVector3 m_initialMomentum;
  std::vector<TVector3> m_trajectory;

  ClassDef(MCEventInformation, 1);

};

#endif // MCEVENTINFORMATION_HH
