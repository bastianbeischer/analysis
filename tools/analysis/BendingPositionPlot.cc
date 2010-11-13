#include "BendingPositionPlot.hh"
#include "BrokenLine.hh"

#include "Hit.hh"

#include <TH1.h>

BendingPositionPlot::BendingPositionPlot()
  : H1DPlot(AnalysisPlot::Tracking, "BendingPositionPlot", 1000, -1000, 1000)
{
}

BendingPositionPlot::~BendingPositionPlot()
{}

void BendingPositionPlot::processEvent(const QVector<Hit*>& clusters, Track* track, SimpleEvent*)
{
  BrokenLine* line = dynamic_cast<BrokenLine*>(track);
  if (line) {
    int nTrackerHits = 0;
    foreach(Hit* hit, clusters)
      if (hit->type() == Hit::tracker)
        ++nTrackerHits;
    if (nTrackerHits != 8)
      return;
    m_histogram->Fill(line->zIntersection());
  }
}

void BendingPositionPlot::finalize()
{
}
