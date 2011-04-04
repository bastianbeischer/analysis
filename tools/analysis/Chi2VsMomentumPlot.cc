#include "Chi2VsMomentumPlot.hh"

#include <cmath>

#include <TH1D.h>
#include <TH2D.h>

#include "Layer.hh"
#include "TrackFinding.hh"
#include "Setup.hh"
#include "Track.hh"
#include "TrackInformation.hh"

Chi2VsMomentumPlot::Chi2VsMomentumPlot() :
  AnalysisPlot(AnalysisPlot::Tracking),
  H2DPlot()
{
  setTitle("Chi2 vs Momentum");
  
  int nBinsX = 10;
  double x0 = 0.;
  double x1 = 10.;
  int nBinsY = 100.;
  double y0 = 0.;
  double y1 = 10.;
  
  TH2D* histogram = new TH2D(qPrintable(title()), "", nBinsX, x0, x1, nBinsY, y0, y1);
  setAxisTitle("R / GV", "#chi^{2} / ndf", "");
  setHistogram(histogram);
}

Chi2VsMomentumPlot::~Chi2VsMomentumPlot()
{
}

void Chi2VsMomentumPlot::processEvent(const QVector<Hit*>&, Track* track, SimpleEvent*)
{
  if(!track || !track->fitGood())
    return;

  TrackInformation::Flags flags = track->information()->flags();
  if (!(flags & TrackInformation::AllTrackerLayers))
    return;

  histogram()->Fill(track->rigidity(), track->chi2() / track->ndf());
}
