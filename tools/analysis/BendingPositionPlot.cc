#include "BendingPositionPlot.hh"
#include "BrokenLine.hh"

#include "ParticleInformation.hh"
#include "Hit.hh"
#include "Particle.hh"
#include "Track.hh"

#include <TH1.h>

BendingPositionPlot::BendingPositionPlot()
  : AnalysisPlot(Enums::Tracking)
  , H1DPlot()
{
  setTitle("bending position");
  TH1D* histogram = new TH1D("bending position", "", 1000, -1000, 1000);
  setAxisTitle("z / mm", "");
  addHistogram(histogram);
  addRequiredEventFlags(Enums::AllTrackerLayers | Enums::TrackGood);
}

BendingPositionPlot::~BendingPositionPlot()
{}

void BendingPositionPlot::processEvent(const AnalyzedEvent* event)
{
  const Track* track = event->particle()->track();
  if (track->type() == Enums::BrokenLine) {
    const BrokenLine* line = static_cast<const BrokenLine*>(track);
    histogram()->Fill(line->zIntersection());
  }
}
