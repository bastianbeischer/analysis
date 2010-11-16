#include "MomentumSpectrumPlot.hh"

#include <TH1D.h>

#include "Hit.hh"
#include "Track.hh"
#include "BrokenLine.hh"
#include "CenteredBrokenLine.hh"

#include <iostream>

MomentumSpectrumPlot::MomentumSpectrumPlot() :
  AnalysisPlot(AnalysisPlot::MomentumReconstruction),
  H1DPlot()
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

  int nTrackerHits = 0;
  foreach(Hit* hit, hits)
    if (hit->type() == Hit::tracker)
      ++nTrackerHits;
  if (nTrackerHits != 8)
    return;

  double pt = track->pt();

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
