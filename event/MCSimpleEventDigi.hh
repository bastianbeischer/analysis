#ifndef MCSIMPLEVENTDIGI_HH
#define MCSIMPLEVENTDIGI_HH

#include <TObject.h>
#include <TVector3.h>
#include <vector>

#include "Hit.hh"

#include "MCDigiSignal.hh"


class MCSimpleEventDigi :
    public TObject
{
public:
  MCSimpleEventDigi(Hit::ModuleType, unsigned short);
  ~MCSimpleEventDigi();

  unsigned short DetID() const {return m_detID;}
  Hit::ModuleType Type() const {return m_type;}
  const std::vector<const MCDigiSignal*>& Signals() const {return m_signals;}

  void AddSignal(const MCDigiSignal* signal) {m_signals.push_back(signal);}


private:
  Hit::ModuleType m_type;
  unsigned short m_detID;
  std::vector<const MCDigiSignal*> m_signals;

  ClassDef(MCSimpleEventDigi,1);

};



#endif // MCSIMPLEVENTDIGI_HH
