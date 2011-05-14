#ifndef MCRigidityResolutionPlot_hh
#define MCRigidityResolutionPlot_hh

#include "RigidityResolutionPlot.hh"

class MCRigidityResolutionPlot :
  public RigidityResolutionPlot
{
  
public:
  MCRigidityResolutionPlot(int pdgID = 11);
  ~MCRigidityResolutionPlot();
  
  void processEvent(const QVector<Hit*>&, Particle* = 0, SimpleEvent* = 0);
  double referenceRigidity(SimpleEvent*) const;
  
};

#endif /* MCRigidityResolutionPlot_hh */
