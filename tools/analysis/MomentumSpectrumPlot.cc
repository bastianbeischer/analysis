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
  TH1D* histogram = 0;
  if (m_range == All) {
    double lowerBound = -20.;
    double upperBound = 20.;
    setTitle(title() + " - All");
    histogram = new TH1D(qPrintable(title()), "", nBins, lowerBound, upperBound);
  }
  else {
    if (m_range == Negative) setTitle(title() + " - Negative");
    if (m_range == Positive) setTitle(title() + " - Positive");
    double lowerBound = 1e-1;
    double upperBound = 20.;
    double delta = 1./nBins * (log(upperBound)/log(lowerBound) - 1);
    double p[nBins+1];
    for (int i = 0; i < nBins+1; i++) {
      p[i] = pow(lowerBound, delta*i+1);
    }
    histogram = new TH1D(qPrintable(title()), "", nBins, p);
  }
  histogram->GetXaxis()->SetTitle("R / GV");
  histogram->GetYaxis()->SetTitle("particles / GV");
  histogram->GetYaxis()->SetTitleOffset(1.3);
  histogram->SetLineColor(kBlack);
  addHistogram(histogram);
  addLatex(RootPlot::newLatex(.15, .85));
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

  if (m_range == Negative)
    pt = -pt;

  int iBin = histogram()->FindBin(pt);
  double width = histogram()->GetBinWidth(iBin);
  histogram()->Fill(pt, 1./width);
}

void MomentumSpectrumPlot::update()
{
  latex(0)->SetTitle(qPrintable(QString("entries = %1").arg(histogram()->GetEntries())));
}
