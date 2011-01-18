#include "AlbedosVsMomentumPlot.hh"

#include <TH1D.h>

#include "Track.hh"
#include "TrackInformation.hh"

AlbedosVsMomentumPlot::AlbedosVsMomentumPlot() :
  AnalysisPlot(AnalysisPlot::MomentumReconstruction)
{
  setTitle("Albedos vs Momentum");

  int nBins = 100;
  TH1D* histogram = 0;
  double lowerBound = -20;
  double upperBound = 20.;
  setTitle(title());
  QString histoTitle = title() + " - All";
  histogram = new TH1D(qPrintable(histoTitle), qPrintable(histoTitle), nBins, lowerBound, upperBound);
  histogram->GetXaxis()->SetTitle("p / GeV");
  histogram->GetYaxis()->SetTitle("entries");
  histogram->SetLineColor(kBlack);
  addHistogram(histogram);
  histoTitle = title() + " - Albedos";
  histogram = new TH1D(qPrintable(histoTitle), qPrintable(histoTitle), nBins, lowerBound, upperBound);
  histogram->GetXaxis()->SetTitle("p / GeV");
  histogram->GetYaxis()->SetTitle("entries");
  histogram->SetLineColor(kRed);
  addHistogram(histogram);

  // TLatex* latex = 0;
  // latex = new TLatex(.15, .85, 0);
  // latex->SetNDC();
  // latex->SetTextAlign(13);
  // latex->SetTextFont(82);
  // latex->SetTextSize(0.03);
  // addLatex(latex);
}

AlbedosVsMomentumPlot::~AlbedosVsMomentumPlot()
{
}

void AlbedosVsMomentumPlot::processEvent(const QVector<Hit*>&, Track* track, SimpleEvent*)
{
    // QMutexLocker locker(&m_mutex);
  if (!track || !track->fitGood())
    return;

  TrackInformation::Flags flags = track->information()->flags();
  if ( !(flags & TrackInformation::AllTrackerLayers) || !(flags & TrackInformation::InsideMagnet) )
    return;

  double pt = track->pt();
  histogram(0)->Fill(pt);

  if (flags & TrackInformation::Albedo)
    histogram(1)->Fill(pt);
}
