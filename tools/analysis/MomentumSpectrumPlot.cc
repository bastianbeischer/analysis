#include "MomentumSpectrumPlot.hh"

#include <TH1D.h>

#include "Hit.hh"
#include "Track.hh"
#include "BrokenLine.hh"
#include "CenteredBrokenLine.hh"

#include <iostream>

MomentumSpectrumPlot::MomentumSpectrumPlot() :
  H1DPlot(AnalysisPlot::MomentumReconstruction)
{
  setTitle("spectrum");
  TH1D* histogram = new TH1D(qPrintable(title()), "", 100, -20, 20);
  histogram->GetXaxis()->SetTitle("p / GeV");
  histogram->GetYaxis()->SetTitle("entries");
  histogram->SetLineColor(kBlack);
  addHistogram(histogram);
  histogram = new TH1D(qPrintable(title()), "", 100, -20, 20);
  histogram->GetXaxis()->SetTitle("p / GeV");
  histogram->GetYaxis()->SetTitle("entries");
  histogram->SetLineColor(kRed);
  addHistogram(histogram);
  histogram = new TH1D(qPrintable(title()), "", 100, -20, 20);
  histogram->GetXaxis()->SetTitle("p / GeV");
  histogram->GetYaxis()->SetTitle("entries");
  histogram->SetLineColor(kBlue);
  addHistogram(histogram);
}

MomentumSpectrumPlot::~MomentumSpectrumPlot()
{
}

void MomentumSpectrumPlot::processEvent(const QVector<Hit*>& hits, Track* track, SimpleEvent* /*event*/)
{
  if (!track || (track->type() != Track::BrokenLine && track->type() != Track::CenteredBrokenLine))
    return;

  double alpha = 0;
  if (track->type() == Track::BrokenLine) {
    alpha = static_cast<BrokenLine*>(track)->bendingAngle();
  }
  if (track->type() == Track::CenteredBrokenLine) {
    alpha = static_cast<CenteredBrokenLine*>(track)->bendingAngle();
  }

  int nTrackerHits = 0;
  foreach(Hit* hit, hits)
    if (hit->type() == Hit::tracker)
      ++nTrackerHits;
  if (nTrackerHits != 8)
    return;

  double z0_magnet = -40.; // mm
  double z1_magnet =  40.; // mm
  double x0_magnet = track->x(z0_magnet); // mm
  double x1_magnet = track->x(z1_magnet); // mm
  double B_estimate = 0.27; // tesla
  double L  = sqrt(pow(x1_magnet - x0_magnet, 2.) + pow(z1_magnet - z0_magnet,2.))*1000.; // convert from mm to m
  double pt = 0.3*B_estimate*L/alpha/1e6; // GeV

  double r = sqrt(track->x(0)*track->x(0) + track->y(0)*track->y(0));
  //  histogram(0)->Fill(pt);
  if (r < 75) {
    histogram(0)->Fill(pt);
  }
  //  if (r > 140)
    //    histogram(2)->Fill(pt);
}

void MomentumSpectrumPlot::finalize()
{
}
