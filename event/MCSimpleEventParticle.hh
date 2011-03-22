#ifndef MCSIMPLEVENTPARTICLE_HH
#define MCSIMPLEVENTPARTICLE_HH

#include <TObject.h>
#include <TVector3.h>
#include <vector>


class MCSimpleEventParticle :
    public TObject
{
public:
  int pdgID;
  TVector3 initialMomentum;
  TVector3 initialPosition;
  std::vector<TVector3> trajectory;

  ClassDef(MCSimpleEventParticle,1);
};

#endif // MCSIMPLEVENTPARTICLE_HH
