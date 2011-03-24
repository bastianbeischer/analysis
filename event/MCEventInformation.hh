#ifndef MCEVENTINFORMATION_HH
#define MCEVENTINFORMATION_HH

#include <TObject.h>
#include <TVector3.h>
#include <vector>
#include "MCSimpleEventParticle.hh"
#include "MCSimpleEventDigi.hh"

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
  const std::vector <const MCSimpleEventDigi*> mcDigis() const {return m_mcDigis;}

  void setPrimary(const MCSimpleEventParticle* prim) { m_primary = prim;}
  void addSecondary(const MCSimpleEventParticle* sec) { m_secondaries.push_back(sec);}
  void addMcDigis(const MCSimpleEventDigi* digi) { m_mcDigis.push_back(digi);}

private:
  const MCSimpleEventParticle* m_primary;
  std::vector <const MCSimpleEventParticle*> m_secondaries;
  std::vector <const MCSimpleEventDigi*> m_mcDigis;

  ClassDef(MCEventInformation,1);

};

#endif // MCEVENTINFORMATION_HH
