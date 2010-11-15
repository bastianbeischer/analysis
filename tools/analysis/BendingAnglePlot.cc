#include "BendingAnglePlot.hh"
#include "BrokenLine.hh"
#include "CenteredBrokenLine.hh"

#include "Hit.hh"

#include <TH1.h>
#include <TCanvas.h>

#include <cmath>

BendingAnglePlot::BendingAnglePlot()
  : H1DPlot(AnalysisPlot::Tracking)
{
  setTitle("bending angle");
  TH1D* histogram = 0;
  histogram = new TH1D("bending angle all tracks", "", 2000, -.2, .2);
  histogram->GetXaxis()->SetTitle("bending angle / rad");
  addHistogram(histogram);
  histogram = new TH1D("bending angle insige magnet", "", 400, -.2, .2);
  histogram->SetLineColor(kRed);
  addHistogram(histogram);
  histogram = new TH1D("bending angle outside magnet", "", 400, -.2, .2);
  histogram->SetLineColor(kBlue);
  addHistogram(histogram);
}

BendingAnglePlot::~BendingAnglePlot()
{}

void BendingAnglePlot::processEvent(const QVector<Hit*>& clusters, Track* track, SimpleEvent*)
{
  double upperSlope = 0, lowerSlope = 0;
  if (track->type() == Track::BrokenLine) {
    upperSlope = static_cast<BrokenLine*>(track)->upperSlopeX();
    lowerSlope = static_cast<BrokenLine*>(track)->lowerSlopeX();
  }
  if (track->type() == Track::CenteredBrokenLine) {
    upperSlope = static_cast<CenteredBrokenLine*>(track)->upperSlopeX();
    lowerSlope = static_cast<CenteredBrokenLine*>(track)->lowerSlopeX();
  }

  if (track->type() == Track::BrokenLine || track->type() == Track::CenteredBrokenLine) {
    int nTrackerHits = 0;
    foreach(Hit* hit, clusters)
      if (hit->type() == Hit::tracker)
        ++nTrackerHits;
    if (nTrackerHits != 8)
      return;
    double alpha = atan(upperSlope) - atan(lowerSlope);
    double r = track->x(0)*track->x(0) + track->y(0)*track->y(0);
    histogram(0)->Fill(alpha);
    if (r < 150)
      histogram(1)->Fill(alpha);
    if (r > 250) {
      histogram(2)->Fill(alpha);
    }

  }
}

void BendingAnglePlot::finalize()
{
}
