#ifndef MCRigidityResolutionPlot_hh
#define MCRigidityResolutionPlot_hh

#include "RigidityResolutionPlot.hh"

#include "Particle.hh"

class MCRigidityResolutionPlot :
  public RigidityResolutionPlot
{
  
public:
  MCRigidityResolutionPlot(const Enums::Particle& = Enums::Electron);
  ~MCRigidityResolutionPlot();
  
  void processEvent(const QVector<Hit*>&, const Particle* const, const SimpleEvent* const);
  double referenceRigidity(const SimpleEvent* const) const;
  
};

#endif /* MCRigidityResolutionPlot_hh */
