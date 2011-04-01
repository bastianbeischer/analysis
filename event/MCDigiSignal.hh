#ifndef MCDIGISIGNAL_HH
#define MCDIGISIGNAL_HH

#include <TObject.h>
#include <TVector3.h>
#include <vector>

#include "Hit.hh"

class MCDigiSignal :
    public TObject
{
public:
  MCDigiSignal();

  double time;
  int trackID;
  TVector3 hitPosition;
  double energyDeposition;

  ClassDef(MCDigiSignal,1);
};


#endif // MCDIGISIGNAL_HH
