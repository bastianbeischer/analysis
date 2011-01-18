#include "AlbedosVsMomentumPlot.hh"

#include <TH1D.h>

#include "Track.hh"
#include "TrackInformation.hh"

AlbedosVsMomentumPlot::AlbedosVsMomentumPlot() :
  AnalysisPlot(AnalysisPlot::MomentumReconstruction),
  H1DPlot(),
  m_albedoHisto(0),
  m_totalHisto(0)
{
  setTitle("Albedos vs Momentum");

  int nBins = 100;
  TH1D* histogram = 0;
  QString histoTitle;
  double lowerBound = -20;
  double upperBound = 20.;
  setTitle(title());

  // ratio histo
  histoTitle = title();
  histogram = new TH1D(qPrintable(histoTitle), qPrintable(histoTitle), nBins, lowerBound, upperBound);
  histogram->GetXaxis()->SetTitle("p / GeV");
  histogram->GetYaxis()->SetTitle("entries");
  histogram->SetLineColor(kBlack);
  addHistogram(histogram);

  // total histo
  histoTitle = title() + " - All";
  histogram = new TH1D(qPrintable(histoTitle), qPrintable(histoTitle), nBins, lowerBound, upperBound);
  histogram->GetXaxis()->SetTitle("p / GeV");
  histogram->GetYaxis()->SetTitle("entries");
  histogram->SetLineColor(kBlack);
  m_totalHisto = histogram;

  // albedo histo
  histoTitle = title() + " - Albedos";
  histogram = new TH1D(qPrintable(histoTitle), qPrintable(histoTitle), nBins, lowerBound, upperBound);
  histogram->GetXaxis()->SetTitle("p / GeV");
  histogram->GetYaxis()->SetTitle("entries");
  histogram->SetLineColor(kRed);
  m_albedoHisto = histogram;

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
  delete m_albedoHisto;
  delete m_totalHisto;
}

void AlbedosVsMomentumPlot::processEvent(const QVector<Hit*>&, Track* track, SimpleEvent*)
{
  if (!track || !track->fitGood())
    return;

  TrackInformation::Flags flags = track->information()->flags();
  if ( !(flags & TrackInformation::AllTrackerLayers) || !(flags & TrackInformation::InsideMagnet) )
    return;

  double pt = track->pt();
  m_totalHisto->Fill(pt);

  if (flags & TrackInformation::Albedo)
    m_albedoHisto->Fill(pt);
}

void AlbedosVsMomentumPlot::update()
{
  TH1D* histo = histogram();
  int nBins = histo->GetNbinsX();
  int threshold = 10;
  for (int iBin = 1; iBin <= nBins; iBin++) {
    double nTotal = m_totalHisto->GetBinContent(iBin);
    if (nTotal < threshold)
      continue;
    double nAlbedos = m_albedoHisto->GetBinContent(iBin);
    histo->SetBinContent(iBin, nAlbedos/nTotal);
  }
}
