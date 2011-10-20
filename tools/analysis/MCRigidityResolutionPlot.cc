#include "MCRigidityResolutionPlot.hh"

#include "SimpleEvent.hh"
#include "ParticleProperties.hh"

MCRigidityResolutionPlot::MCRigidityResolutionPlot(const Enums::Particle& type) :
  RigidityResolutionPlot(Enums::MonteCarloTracker, type)
{
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
  if (event->simpleEvent()->MCInformation()->primary()->pdgID != (m_particle->pdgId()))
    return;

  RigidityResolutionPlot::processEvent(event);
}

double MCRigidityResolutionPlot::referenceRigidity(const SimpleEvent* const event) const
{
  //get mc rigidity
  double genMom = 0;
  double genRigidity = 0;
  genMom = event->MCInformation()->primary()->initialMomentum.Mag();
  genRigidity = genMom / m_particle->charge();
  return genRigidity;
}
