#include "BendingPositionPlot.hh"
#include "BrokenLine.hh"

#include "Hit.hh"

#include <TH1.h>

BendingPositionPlot::BendingPositionPlot()
  : H1DPlot(AnalysisPlot::Tracking)
{
  setTitle("bending position");
  TH1D* histogram = new TH1D("bending position", "", 1000, -1000, 1000);
  histogram->GetXaxis()->SetTitle("z / mm");
  addHistogram(histogram);
}

BendingPositionPlot::~BendingPositionPlot()
{}

void BendingPositionPlot::processEvent(const QVector<Hit*>& clusters, Track* track, SimpleEvent*)
{
  if (track->type() == Track::BrokenLine) {
    BrokenLine* line = static_cast<BrokenLine*>(track);
    int nTrackerHits = 0;
    foreach(Hit* hit, clusters)
      if (hit->type() == Hit::tracker)
        ++nTrackerHits;
    if (nTrackerHits != 8)
      return;
    histogram(0)->Fill(line->zIntersection());
  }
}

void BendingPositionPlot::finalize()
{
}