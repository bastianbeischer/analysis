#include "Chi2Plot.hh"

#include <TH1D.h>

#include "Track.hh"

Chi2Plot::Chi2Plot() :
  H1DPlot(AnalysisPlot::Tracking)
{
  setTitle("chi2");
  addHistogram(new TH1D("chi2", "", 1000, 0., 100.));
}

Chi2Plot::~Chi2Plot()
{
}

void Chi2Plot::processEvent(const QVector<Hit*>& /*hits*/, Track* track, SimpleEvent* /*event*/)
{
  if (track) {
    histogram(0)->Fill(track->chi2());
  }
}

void Chi2Plot::finalize()
{
}
