#include "MCSimpleEventParticle.hh"

#include <cmath>

MCSimpleEventParticle::MCSimpleEventParticle()
  : TObject()
{
}

double MCSimpleEventParticle::zenithAngle() const
{
  const double x = -initialMomentum.X();
  const double y = -initialMomentum.Y();
  const double z = -initialMomentum.Z();
  return atan(sqrt(pow(x, 2) + pow(y, 2)) / z);
}

double MCSimpleEventParticle::azimuthAngle() const
{
  //with respect to the x-axis
  const double y = -initialMomentum.Y();
  const double x = -initialMomentum.X();
  
  if (x > 0) {
    return atan(y / x);
  } else if (x < 0) {
    if (y >= 0) {
      return atan(y / x) + M_PI;
    } else {
      return atan(y / x) - M_PI;
    }
  } else {
    if (y > 0) {
      return M_PI / 2.;
    } else if (y < 0) {
      return -M_PI / 2.;
    } else {
      return NAN;
    }
  }
}

bool MCSimpleEventParticle::isInsideMagnet() const
{
  const double zMagentUp = 40.;
  const double zMagnetLow = -40.;
  double x = 0;
  double y = 0;
  
  if (!(trajectoryPosition(zMagentUp, x, y) && trajectoryPosition(zMagnetLow, x, y)) ) {
    return false;
  }
  
  double r1 = sqrt(pow(x, 2.) + pow(y, 2.));
  double r2 = sqrt(pow(x, 2.) + pow(y, 2.));
  if (r1 < 75. && r2 < 75.) {
    return true;
  } else {
    return false;
  }
  
}

bool MCSimpleEventParticle::trajectoryPosition(const double z, double& x, double& y) const
{
  for (unsigned int i = 0; i < trajectory.size()-1; ++i) {
    const TVector3& a = trajectory[i];
    const TVector3& b = trajectory[i+1];
    if (a.z() >= z && z >= b.z()) {
      if (b.z() - a.z() != 0) {
        double r = (z - a.z()) / (b.z() - a.z());
        TVector3 line = a + r *(b - a);
        x = line.x();
        y = line.y();
      } else {
        x = a.x();
        y = a.y();
      }
    }
  }
  //returns false if z is no on trajectory, otherwise true;
  if (trajectory[0].z() >= z && z >= trajectory[trajectory.size()-1].z()) {
    return true;
  } else {
    return false;
  }
}

ClassImp(MCSimpleEventParticle);
