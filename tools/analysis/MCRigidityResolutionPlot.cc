#include "MCRigidityResolutionPlot.hh"

#include "SimpleEvent.hh"
#include "ParticleProperties.hh"

MCRigidityResolutionPlot::MCRigidityResolutionPlot(int pdgID) :
  RigidityResolutionPlot(AnalysisPlot::MonteCarloTracker, pdgID)
{
  setTitle("mc rigidity resolution for " + m_particle->name());
  initialize();
}

MCRigidityResolutionPlot::~MCRigidityResolutionPlot()
{
}

void MCRigidityResolutionPlot::processEvent(const QVector<Hit*>& hits, Particle* particle, SimpleEvent* event)
{
  //only accept mc events:
  if (event->contentType() != SimpleEvent::MonteCarlo)
    return;

  //only use selected pdgID for MCEvents:
  if (event->MCInformation()->primary()->pdgID != (m_particle->pdgId()))
    return;

  RigidityResolutionPlot::processEvent(hits, particle, event);
}

double MCRigidityResolutionPlot::referenceRigidity(SimpleEvent* event) const
{
  //get mc rigidity
  double genMom = 0;
  double genRigidity = 0;
  genMom = event->MCInformation()->primary()->initialMomentum.Mag();
  genRigidity = genMom / m_particle->charge();
  return genRigidity;
}
