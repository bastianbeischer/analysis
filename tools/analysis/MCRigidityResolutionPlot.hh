#ifndef MCRigidityResolutionPlot_hh
#define MCRigidityResolutionPlot_hh

#include "RigidityResolutionPlot.hh"

#include "Particle.hh"

class MCRigidityResolutionPlot : public RigidityResolutionPlot {
public:
  MCRigidityResolutionPlot(const Enums::Particle& = Enums::Electron);
  ~MCRigidityResolutionPlot();
  void processEvent(const AnalyzedEvent*);
protected:
  double referenceRigidity(const AnalyzedEvent*) const;
};

#endif /* MCRigidityResolutionPlot_hh */
