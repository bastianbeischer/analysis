#ifndef MCSIMPLEVENTPARTICLE_HH
#define MCSIMPLEVENTPARTICLE_HH

#include <TObject.h>
#include <TVector3.h>
#include <vector>


class MCSimpleEventParticle :
    public TObject
{
public:
  MCSimpleEventParticle();
  int pdgID;
  int trackID;
  int parentTrackID;
  TVector3 initialMomentum;
  TVector3 initialPosition;
  std::vector<TVector3> trajectory;

  double zenithAngle() const;
  double azimuthAngle() const;
  bool isInsideMagnet() const;
  bool trajectoryPosition(const double z, double& x, double& y) const;

  ClassDef(MCSimpleEventParticle,1);
};

#endif // MCSIMPLEVENTPARTICLE_HH
