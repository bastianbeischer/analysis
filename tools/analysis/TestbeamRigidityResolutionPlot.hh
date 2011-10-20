#ifndef TestbeamRigidityResolutionPlot_hh
#define TestbeamRigidityResolutionPlot_hh

#include "RigidityResolutionPlot.hh"

class Settings;

class TestbeamRigidityResolutionPlot : public RigidityResolutionPlot {
public:
  TestbeamRigidityResolutionPlot(const Enums::Particle& = Enums::Electron);
  ~TestbeamRigidityResolutionPlot();
  void processEvent(const AnalyzedEvent*);
  double referenceRigidity(const AnalyzedEvent*) const;
};

#endif /* TestbeamRigidityResolutionPlot_hh */
