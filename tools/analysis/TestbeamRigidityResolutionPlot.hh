#ifndef TestbeamRigidityResolutionPlot_hh
#define TestbeamRigidityResolutionPlot_hh

#include "RigidityResolutionPlot.hh"

class TestbeamRigidityResolutionPlot :
  public RigidityResolutionPlot
{
  
public:
  TestbeamRigidityResolutionPlot(const Particle::Type& = Particle::Electron);
  ~TestbeamRigidityResolutionPlot();
  
  void processEvent(const QVector<Hit*>&, Particle* = 0, SimpleEvent* = 0);
  double referenceRigidity(SimpleEvent*) const;
  
};

#endif /* TestbeamRigidityResolutionPlot_hh */
