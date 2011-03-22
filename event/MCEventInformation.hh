#ifndef MCEVENTINFORMATION_HH
#define MCEVENTINFORMATION_HH

#include <TObject.h>
#include <TVector3.h>
#include <vector>
#include "MCSimpleEventParticle.hh"

class MCEventInformation :
  public TObject
{
public:
  MCEventInformation();
  //MCEventInformation(const MCEventInformation& other);

  int pdgId() const {return m_primary.pdgID;}
  TVector3 initialMomentum() const {return m_primary.initialMomentum;}
  std::vector<TVector3> trajectory() const {return m_primary.trajectory;}
  MCSimpleEventParticle primary() const {return m_primary;}
  std::vector<MCSimpleEventParticle> secondaries() const {return m_secondaries;}

  void setPdgId(int pdgId) {m_primary.pdgID = pdgId;}
  void setInitialMomentum(const TVector3& initialMomentum) {m_primary.initialMomentum = initialMomentum;}
  void setTrajectory(const std::vector<TVector3>& traj) {m_primary.trajectory = traj;}
  void setPrimary(const MCSimpleEventParticle& prim) { m_primary = prim;}
  void addSecondary(const MCSimpleEventParticle& sec) { m_secondaries.push_back(sec);}

private:
  MCSimpleEventParticle m_primary;
  std::vector<MCSimpleEventParticle> m_secondaries;

  ClassDef(MCEventInformation,1);

};

#endif // MCEVENTINFORMATION_HH
