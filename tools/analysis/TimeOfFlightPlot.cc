#include "TimeOfFlightPlot.hh"
#include "BrokenLine.hh"

#include "Hit.hh"

#include <TH1.h>

TimeOfFlightPlot::TimeOfFlightPlot()
  : AnalysisPlot(AnalysisPlot::MiscellaneousTOF)
  , H1DPlot()
{
  setTitle("time of flight");
  TH1D* histogram = new TH1D("time of flight", "", 100, -10, 10);
  histogram->GetXaxis()->SetTitle("time of flight / ns");
  addHistogram(histogram);
}

TimeOfFlightPlot::~TimeOfFlightPlot()
{}

void TimeOfFlightPlot::processEvent(const QVector<Hit*>& clusters, Track* track, SimpleEvent*)
{
  QMutexLocker locker(&m_mutex);
  if (track->type() == Track::BrokenLine) {
    //BrokenLine* line = static_cast<BrokenLine*>(track);
    int nTrackerHits = 0;
    foreach(Hit* hit, clusters)
      if (hit->type() == Hit::tracker)
        ++nTrackerHits;
    if (nTrackerHits != 8)
      return;
    //histogram(0)->Fill(line->zIntersection());
  }
}

void TimeOfFlightPlot::finalize()
{
}
