#ifndef TestbeamRigidityResolutionPlot_hh
#define TestbeamRigidityResolutionPlot_hh

#include "RigidityResolutionPlot.hh"

class TestbeamRigidityResolutionPlot :
  public RigidityResolutionPlot
{
  
public:
  TestbeamRigidityResolutionPlot(const Enums::Particle& = Enums::Electron);
  ~TestbeamRigidityResolutionPlot();
  
  void processEvent(const QVector<Hit*>&, const Particle* const, const SimpleEvent* const);
  double referenceRigidity(const SimpleEvent* const) const;
  
};

#endif /* TestbeamRigidityResolutionPlot_hh */
