#include "BendingPositionPlot.hh"
#include "BrokenLine.hh"

#include "TrackSelection.hh"
#include "Hit.hh"

#include <TH1.h>

BendingPositionPlot::BendingPositionPlot()
  : AnalysisPlot(AnalysisPlot::Tracking)
  , H1DPlot()
{
  setTitle("bending position");
  TH1D* histogram = new TH1D("bending position", "", 1000, -1000, 1000);
  histogram->GetXaxis()->SetTitle("z / mm");
  addHistogram(histogram);
}

BendingPositionPlot::~BendingPositionPlot()
{}

void BendingPositionPlot::processEvent(const QVector<Hit*>&, Track* track, TrackSelection* selection, SimpleEvent*)
{
  // QMutexLocker locker(&m_mutex);
  if(!track || !selection || !track->fitGood())
    return;

  if (track->type() == Track::BrokenLine) {
    BrokenLine* line = static_cast<BrokenLine*>(track);

    TrackSelection::Flags flags = selection->flags();
    if (!(flags & TrackSelection::AllTrackerLayers))
      return;

    histogram(0)->Fill(line->zIntersection());
  }
}
