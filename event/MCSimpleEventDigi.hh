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
  MCSimpleEventDigi();
  MCSimpleEventDigi(Hit::ModuleType, unsigned short);
  MCSimpleEventDigi(const MCSimpleEventDigi&);
  ~MCSimpleEventDigi();

  unsigned short detID() const {return m_detID;}
  Hit::ModuleType type() const {return m_type;}
  const std::vector<const MCDigiSignal*>& digiSignals() const {return m_signals;}

  void addSignal(const MCDigiSignal* signal) {m_signals.push_back(signal);}
  void setDetID(unsigned short id) {m_detID = id;}
  void setType(Hit::ModuleType type) {m_type = type;}

  double signalSum() const;


private:
  Hit::ModuleType m_type;
  unsigned short m_detID;
  std::vector<const MCDigiSignal*> m_signals;

  ClassDef(MCSimpleEventDigi,1);

};

#endif // MCSIMPLEVENTDIGI_HH
