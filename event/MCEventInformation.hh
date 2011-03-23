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
  ~MCEventInformation();
  MCEventInformation(const MCEventInformation& other);

public:
  const MCSimpleEventParticle* primary() const {return m_primary;}
  const std::vector <const MCSimpleEventParticle*> secondaries() const {return m_secondaries;}

  void setPrimary(const MCSimpleEventParticle* prim) { m_primary = prim;}
  void addSecondary(const MCSimpleEventParticle* sec) { m_secondaries.push_back(sec);}

private:
  const MCSimpleEventParticle* m_primary;
  std::vector <const MCSimpleEventParticle*> m_secondaries;

  ClassDef(MCEventInformation,1);

};

#endif // MCEVENTINFORMATION_HH
