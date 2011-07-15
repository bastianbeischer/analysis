#include "BendingPositionPlot.hh"
#include "BrokenLine.hh"

#include "ParticleInformation.hh"
#include "Hit.hh"
#include "Particle.hh"
#include "Track.hh"

#include <TH1.h>

BendingPositionPlot::BendingPositionPlot()
  : AnalysisPlot(AnalysisPlot::Tracking)
  , H1DPlot()
{
  setTitle("bending position");
  TH1D* histogram = new TH1D("bending position", "", 1000, -1000, 1000);
  setAxisTitle("z / mm", "");
  addHistogram(histogram);
}

BendingPositionPlot::~BendingPositionPlot()
{}

void BendingPositionPlot::processEvent(const QVector<Hit*>&, Particle* particle, SimpleEvent*)
{
  const Track* track = particle->track();

  if(!track || !track->fitGood())
    return;

  if (track->type() == Track::BrokenLine) {
    const BrokenLine* line = static_cast<const BrokenLine*>(track);

    ParticleInformation::Flags flags = particle->information()->flags();
    if (!(flags & ParticleInformation::AllTrackerLayers))
      return;

    histogram()->Fill(line->zIntersection());
  }
}
