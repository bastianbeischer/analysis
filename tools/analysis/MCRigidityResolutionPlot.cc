#include "MCRigidityResolutionPlot.hh"

#include "SimpleEvent.hh"
#include "ParticleProperties.hh"

MCRigidityResolutionPlot::MCRigidityResolutionPlot(const Enums::Particle& type)
  : RigidityResolutionPlot(Enums::MonteCarloTracker, type)
{
  setTitle("MC " + title());
}

MCRigidityResolutionPlot::~MCRigidityResolutionPlot()
{
}

void MCRigidityResolutionPlot::processEvent(const AnalyzedEvent* event)
{
  //only accept mc events:
  if (event->simpleEvent()->contentType() != SimpleEvent::MonteCarlo)
    return;

  //only use selected pdgID for MCEvents:
  if (event->simpleEvent()->MCInformation()->primary()->pdgID != (m_particleProperties.pdgId()))
    return;

  RigidityResolutionPlot::processEvent(event);
}

double MCRigidityResolutionPlot::referenceRigidity(const AnalyzedEvent* event) const
{
  //get mc rigidity
  double p = event->simpleEvent()->MCInformation()->primary()->initialMomentum.Mag();
  int z = m_particleProperties.charge();
  return p / z;
}
