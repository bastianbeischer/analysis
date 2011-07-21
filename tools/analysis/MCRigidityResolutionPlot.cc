#include "MCRigidityResolutionPlot.hh"

#include "SimpleEvent.hh"
#include "ParticleProperties.hh"

MCRigidityResolutionPlot::MCRigidityResolutionPlot(const Particle::Type& type) :
  RigidityResolutionPlot(AnalysisPlot::MonteCarloTracker, type)
{
}

MCRigidityResolutionPlot::~MCRigidityResolutionPlot()
{
}

void MCRigidityResolutionPlot::processEvent(const QVector<Hit*>& hits, const Particle* const particle, const SimpleEvent* const event)
{
  //only accept mc events:
  if (event->contentType() != SimpleEvent::MonteCarlo)
    return;

  //only use selected pdgID for MCEvents:
  if (event->MCInformation()->primary()->pdgID != (m_particle->pdgId()))
    return;

  RigidityResolutionPlot::processEvent(hits, particle, event);
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
