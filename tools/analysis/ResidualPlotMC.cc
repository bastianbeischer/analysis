#include "ResidualPlotMC.hh"

#include "SimpleEvent.hh"
#include "Particle.hh"
#include "Track.hh"
#include "StraightLine.hh"

#include <TVector3.h>

ResidualPlotMC::ResidualPlotMC(AnalysisTopic topic, Layer* layer)
  : ResidualPlot(topic, layer)
{
  setTitle("MC " + title());
}

Track* ResidualPlotMC::referenceTrack(const QVector<Hit*>&, const Particle* const, const SimpleEvent* const event)
{
  if (event->contentType() != SimpleEvent::MonteCarlo)
    return 0;

  StraightLine* mcTrack = new StraightLine();
  TVector3 dir = event->MCInformation()->primary()->initialMomentum.Unit();
  TVector3 pos = event->MCInformation()->primary()->initialPosition;
  double factorTo0 = (-1.)* pos.z()/dir.z();
  TVector3 pos0 = pos + factorTo0*dir;
  mcTrack->setX0(pos0.x());
  mcTrack->setY0(pos0.y());
  mcTrack->setSlopeX(dir.x() / dir.z());
  mcTrack->setSlopeY(dir.y() / dir.z());

  return mcTrack;
}
