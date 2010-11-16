#include "Chi2Plot.hh"

#include <TH1D.h>

#include "Hit.hh"
#include "Track.hh"

Chi2Plot::Chi2Plot() :
  H1DPlot(AnalysisPlot::Tracking)
{
  setTitle("chi2");
  TH1D* histogram = new TH1D("chi2", "", 250, 0., 100.);
  histogram->GetXaxis()->SetTitle("#chi^{2}");
  histogram->GetYaxis()->SetTitle("entries");
  addHistogram(histogram);
}

Chi2Plot::~Chi2Plot()
{
}

void Chi2Plot::processEvent(const QVector<Hit*>& hits, Track* track, SimpleEvent* /*event*/)
{
  int nTrackerHits = 0;
  foreach(Hit* hit, hits)
    if (hit->type() == Hit::tracker)
      ++nTrackerHits;
  if (nTrackerHits != 8)
    return;

  if (track) {
    histogram(0)->Fill(track->chi2());
  }
}

void Chi2Plot::finalize()
{
}
