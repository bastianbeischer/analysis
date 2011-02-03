#include "Chi2PerNdfPlot.hh"

#include <TH1D.h>

#include "TrackInformation.hh"
#include "Hit.hh"
#include "Track.hh"

Chi2PerNdfPlot::Chi2PerNdfPlot() :
  AnalysisPlot(AnalysisPlot::Tracking),
  H1DPlot()
{
  setTitle("chi2 / ndf");
  int nBins = 100;
  double x0 = 0.;
  double x1 = 10.;

  TH1D* histogram = new TH1D(qPrintable(title()), "", nBins, x0, x1);
  histogram->GetXaxis()->SetTitle("#chi^{2} / ndf");
  histogram->GetYaxis()->SetTitle("entries");
  addHistogram(histogram);
}

Chi2PerNdfPlot::~Chi2PerNdfPlot()
{
}

void Chi2PerNdfPlot::processEvent(const QVector<Hit*>&, Track* track, SimpleEvent* /*event*/)
{
  // QMutexLocker locker(&m_mutex);
  if(!track || !track->fitGood())
    return;

  TrackInformation::Flags flags = track->information()->flags();
  if (!(flags & TrackInformation::AllTrackerLayers))
    return;

  histogram()->Fill(track->chi2() / track->ndf());
}
