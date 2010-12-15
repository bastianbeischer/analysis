#include "Chi2Plot.hh"

#include <TH1D.h>

#include "TrackInformation.hh"
#include "Hit.hh"
#include "Track.hh"

Chi2Plot::Chi2Plot() :
  AnalysisPlot(AnalysisPlot::Tracking),
  H1DPlot()
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

void Chi2Plot::processEvent(const QVector<Hit*>&, Track* track, SimpleEvent* /*event*/)
{
  // QMutexLocker locker(&m_mutex);
  if(!track || !track->fitGood())
    return;

  TrackInformation::Flags flags = track->information()->flags();
  if (!(flags & TrackInformation::AllTrackerLayers))
    return;

  if ((int)track->ndf() == 20 - track->nParameters()) {
    histogram()->Fill(track->chi2());
  }
}
