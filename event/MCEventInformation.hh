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
  MCEventInformation(const MCEventInformation& other):TObject()
  {
    *this = other;
  }

  int PDGid() const                                 { return m_pdgID; }
  void PDGid(int pdgID)                             { m_pdgID = pdgID; }

  TVector3 InitialMom() const                       { return m_initialMomentum; }
  void InitialMom(const TVector3& initialMom)       { m_initialMomentum = initialMom; }

  std::vector<TVector3> Trajectory() const          { return m_trajectory; }
  void Trajectory(const std::vector<TVector3> traj)  { m_trajectory = traj; }

private:
  int m_pdgID;
  TVector3 m_initialMomentum;
  std::vector<TVector3> m_trajectory;

  ClassDef( MCEventInformation, 1 );

};

#endif // MCEVENTINFORMATION_HH
