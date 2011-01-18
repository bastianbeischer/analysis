#include "MomentumSpectrumPlot.hh"

#include <TH1D.h>
#include <TLatex.h>

#include "Hit.hh"
#include "TrackInformation.hh"
#include "Track.hh"

#include <iostream>
#include <cmath>

MomentumSpectrumPlot::MomentumSpectrumPlot(Range range) :
  AnalysisPlot(AnalysisPlot::MomentumReconstruction),
  H1DPlot(),
  m_range(range)
{
  setTitle("Momentum Spectrum");

  int nBins = 100;
  double lowerBound = -20;
  double upperBound = 20;
  TH1D* histogram = 0;
  if (m_range == All) {
    setTitle(title() + " - All");
    histogram = new TH1D(qPrintable(title()), "", nBins, lowerBound, upperBound);
  }
  else if (m_range == Negative) {
    setTitle(title() + " - Negative");
    upperBound = -1e-5;
    histogram = new TH1D(qPrintable(title()), "", nBins, lowerBound, upperBound);
  }
  else if (m_range == Positive) {
    setTitle(title() + " - Positive");
    lowerBound = 1e-1;
    double delta = 1./nBins * (log(upperBound)/log(lowerBound) - 1);
    double p[nBins+1];
    for (int i = 0; i < nBins+1; i++) {
      p[i] = pow(lowerBound, delta*i+1);
    }
    histogram = new TH1D(qPrintable(title()), "", nBins, p);
  }
  histogram->GetXaxis()->SetTitle("p / GeV");
  histogram->GetYaxis()->SetTitle("entries");
  histogram->SetLineColor(kBlack);
  addHistogram(histogram);
  TLatex* latex = 0;
  latex = new TLatex(.15, .85, 0);
  latex->SetNDC();
  latex->SetTextAlign(13);
  latex->SetTextFont(82);
  latex->SetTextSize(0.03);
  addLatex(latex);
}

MomentumSpectrumPlot::~MomentumSpectrumPlot()
{
}

void MomentumSpectrumPlot::processEvent(const QVector<Hit*>&, Track* track, SimpleEvent* /*event*/)
{
  // QMutexLocker locker(&m_mutex);
  if (!track || !track->fitGood())
    return;

  TrackInformation::Flags flags = track->information()->flags();
  if ( !(flags & TrackInformation::AllTrackerLayers) || !(flags & TrackInformation::InsideMagnet) || (flags & TrackInformation::Albedo) )
    return;

  double pt = track->pt();
  if (m_range == Negative && pt >= 0)
    return;
  if (m_range == Positive && pt <= 0)
    return;
  histogram()->Fill(pt);
}

void MomentumSpectrumPlot::update()
{
  latex(0)->SetTitle(qPrintable(QString("entries = %1").arg(histogram()->GetEntries())));
}
