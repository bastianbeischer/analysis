#include "AlbedosVsMomentumPlot.hh"

#include <TH1D.h>

#include "Particle.hh"
#include "Track.hh"
#include "ParticleInformation.hh"

AlbedosVsMomentumPlot::AlbedosVsMomentumPlot() :
  AnalysisPlot(AnalysisTopic::MomentumReconstruction),
  H1DPlot(),
  m_albedoHisto(0),
  m_totalHisto(0)
{
  setTitle("Albedos vs Momentum");
  setAxisTitle("R / GV", "");

  int nBins = 100;
  TH1D* histogram = 0;
  QString histoTitle;
  double lowerBound = -20;
  double upperBound = 20.;
  setTitle(title());

  // ratio histo
  histoTitle = title();
  histogram = new TH1D(qPrintable(histoTitle), qPrintable(histoTitle), nBins, lowerBound, upperBound);
  histogram->SetLineColor(kBlack);
  addHistogram(histogram);

  // total histo
  histoTitle = title() + " - All";
  histogram = new TH1D(qPrintable(histoTitle), qPrintable(histoTitle), nBins, lowerBound, upperBound);
  histogram->SetLineColor(kBlack);
  m_totalHisto = histogram;

  // albedo histo
  histoTitle = title() + " - Albedos";
  histogram = new TH1D(qPrintable(histoTitle), qPrintable(histoTitle), nBins, lowerBound, upperBound);
  histogram->SetLineColor(kRed);
  m_albedoHisto = histogram;
}

AlbedosVsMomentumPlot::~AlbedosVsMomentumPlot()
{
  delete m_albedoHisto;
  delete m_totalHisto;
}

void AlbedosVsMomentumPlot::processEvent(const QVector<Hit*>&, const Particle* const particle, const SimpleEvent* const)
{
  const Track* track = particle->track();

  if (!track || !track->fitGood())
    return;

  ParticleInformation::Flags flags = particle->information()->flags();
  ParticleInformation::Flags required = ParticleInformation::Chi2Good | ParticleInformation::InsideMagnet | ParticleInformation::BetaGood;
  if ((flags & required) != required)
    return;

  double rigidity = track->rigidity();
  m_totalHisto->Fill(rigidity);

  if (flags & ParticleInformation::Albedo)
    m_albedoHisto->Fill(rigidity);
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
