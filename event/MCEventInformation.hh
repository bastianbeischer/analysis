#ifndef MCEVENTINFORMATION_HH
#define MCEVENTINFORMATION_HH

#include <TObject.h>
#include <TVector3.h>

class MCEventInformation  :
    public TObject
{
public:
  MCEventInformation();

  int PDGid() const                             { return m_pdgID; }
  void PDGid(int pdgID)                         { m_pdgID = pdgID; }

  const TVector3& InitialMom() const            { return m_initialMomentum; }
  void InitialMom(const TVector3& initialMom)   { m_initialMomentum = initialMom; }

private:
  int m_pdgID;
  TVector3 m_initialMomentum;

  ClassDef( MCEventInformation, 1 );

};

#endif // MCEVENTINFORMATION_HH
