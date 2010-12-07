#include "MomentumSpectrumPlot.hh"

#include <TH1D.h>

#include "Hit.hh"
#include "TrackSelection.hh"
#include "Track.hh"
#include "BrokenLine.hh"
#include "CenteredBrokenLine.hh"

#include <iostream>

MomentumSpectrumPlot::MomentumSpectrumPlot(Range range) :
  AnalysisPlot(AnalysisPlot::MomentumReconstruction),
  H1DPlot(),
  m_range(range)
{
  setTitle("Momentum Spectrum");

  int lowerBound = -20;
  int upperBound = 20;
  if (m_range == All) {
    setTitle(title() + " - All");
  }
  else if (m_range == Negative) {
    setTitle(title() + " - Negative");
    upperBound = -1e-5;
  }
  else if (m_range == Positive) {
    setTitle(title() + " - Positive");
    lowerBound = 1e-5;
  }
  TH1D* histogram = new TH1D(qPrintable(title()), "", 100, lowerBound, upperBound);
  histogram->GetXaxis()->SetTitle("p / GeV");
  histogram->GetYaxis()->SetTitle("entries");
  histogram->SetLineColor(kBlack);
  addHistogram(histogram);
}

MomentumSpectrumPlot::~MomentumSpectrumPlot()
{
}

void MomentumSpectrumPlot::processEvent(const QVector<Hit*>&, Track* track, TrackSelection* selection, SimpleEvent* /*event*/)
{
  // QMutexLocker locker(&m_mutex);
  if (!track || !selection || !track->fitGood())
    return;

  TrackSelection::Flags flags = selection->flags();
  if (!(flags & TrackSelection::AllTrackerLayers))
    return;

  double pt = track->pt();

  if (flags & TrackSelection::InsideMagnet) {
    if (m_range == Negative && pt >= 0)
      return;
    if (m_range == Positive && pt <= 0)
      return;
    histogram(0)->Fill(pt);
  }
}
