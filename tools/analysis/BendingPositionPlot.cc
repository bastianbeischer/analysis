#include "BendingPositionPlot.hh"
#include "BrokenLine.hh"

#include "TrackInformation.hh"
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

  // QMutexLocker locker(&m_mutex);
  if(!track || !track->fitGood())
    return;

  if (track->type() == Track::BrokenLine) {
    const BrokenLine* line = static_cast<const BrokenLine*>(track);

    TrackInformation::Flags flags = track->information()->flags();
    if (!(flags & TrackInformation::AllTrackerLayers))
      return;

    histogram()->Fill(line->zIntersection());
  }
}
