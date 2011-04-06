#include "MomentumSpectrumPlot.hh"

#include <TH1D.h>
#include <TLatex.h>

#include "Hit.hh"
#include "TrackInformation.hh"
#include "Particle.hh"
#include "Track.hh"

#include <iostream>
#include <cmath>

MomentumSpectrumPlot::MomentumSpectrumPlot(Type type) :
  AnalysisPlot(AnalysisPlot::MomentumReconstruction),
  H1DPlot(),
  m_type(type)
{
  setTitle("Momentum Spectrum");
  int nBins = 100;
  TH1D* histogram = 0;
  if (m_type == All) {
    double lowerBound = -20.;
    double upperBound = 20.;
    setTitle(title() + " - All");
    histogram = new TH1D(qPrintable(title()), "", nBins, lowerBound, upperBound);
    setAxisTitle("R / GV", "particles / GV");
  } else if (m_type == Negative || m_type == Positive) {
    if (m_type == Negative) setTitle(title() + " - Negative");
    if (m_type == Positive) setTitle(title() + " - Positive");
    double lowerBound = 1e-1;
    double upperBound = 20.;
    double delta = 1./nBins * (log(upperBound)/log(lowerBound) - 1);
    double p[nBins+1];
    for (int i = 0; i < nBins+1; i++) {
      p[i] = pow(lowerBound, delta*i+1);
    }
    histogram = new TH1D(qPrintable(title()), "", nBins, p);
    setAxisTitle("R / GV", "particles / GV");
  } else if (m_type == Inverted) {
    setTitle(title() + " - Inverted");
    nBins = 300;
    double lowerBound = -10;
    double upperBound = 10;
    histogram = new TH1D(qPrintable(title()), "", nBins, lowerBound, upperBound);
    setAxisTitle("1/R / 1/GV", "particles / bin");
  }
  
  histogram->SetLineColor(kBlack);
  addHistogram(histogram);
  addLatex(RootPlot::newLatex(.15, .85));
}

MomentumSpectrumPlot::~MomentumSpectrumPlot()
{
}

void MomentumSpectrumPlot::processEvent(const QVector<Hit*>&, Particle* particle, SimpleEvent* /*event*/)
{
  const Track* track = particle->track();

  // QMutexLocker locker(&m_mutex);
  if (!track || !track->fitGood())
    return;

  TrackInformation::Flags flags = track->information()->flags();
  if ( !(flags & TrackInformation::AllTrackerLayers) || !(flags & TrackInformation::InsideMagnet) || (flags & TrackInformation::Albedo) )
    return;

  double rigidity = track->rigidity();
  if (m_type == Negative && rigidity >= 0)
    return;
  if (m_type == Positive && rigidity <= 0)
    return;

  double value = rigidity;
  if (m_type == Negative) {
    value = -rigidity;
  }
  else if (m_type == Inverted) {
    value = 1./rigidity;
  }

  int iBin = histogram()->FindBin(value);
  double width = histogram()->GetBinWidth(iBin);
  double weight = 1./width;
  if (m_type == Inverted) {
    weight = 1.;
  }

  histogram()->Fill(value, weight);
}

void MomentumSpectrumPlot::update()
{
  latex(0)->SetTitle(qPrintable(QString("entries = %1").arg(histogram()->GetEntries())));
}
