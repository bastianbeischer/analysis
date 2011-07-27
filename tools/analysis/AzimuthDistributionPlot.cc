#include "AzimuthDistributionPlot.hh"

#include <QDebug>

#include <TH1D.h>
#include <TLatex.h>

#include "Hit.hh"
#include "ParticleInformation.hh"
#include "Particle.hh"
#include "Track.hh"
#include "SimpleEvent.hh"

#include <iostream>
#include <cmath>

AzimuthDistributionPlot::AzimuthDistributionPlot()
  : AnalysisPlot(AnalysisPlot::Tracking)
  , H1DPlot()
{
  setTitle("Azimuth distribution");
  int nBins = 45;
  double xMin = -180;
  double xMax = 180;
  TH1D* histogram = new TH1D(qPrintable(title()), "", nBins, xMin, xMax);
  histogram->Sumw2();
  setAxisTitle("azimuth", "");
  addHistogram(histogram);
  addLatex(RootPlot::newLatex(.15, .85));
}

AzimuthDistributionPlot::~AzimuthDistributionPlot()
{
}

void AzimuthDistributionPlot::processEvent(const QVector<Hit*>&, const Particle* const particle, const SimpleEvent* const)
{
  const Track* track = particle->track();

  if (!track || !track->fitGood())
    return;

  ParticleInformation::Flags flags = particle->information()->flags();
  if ( !(flags & ParticleInformation::Chi2Good) || !(flags & ParticleInformation::InsideMagnet) || !(flags & ParticleInformation::BetaGood) || (flags & ParticleInformation::Albedo) )
    return;

  double azimuth = track->azimuthAngle() * 180. / M_PI;

  histogram()->Fill(azimuth);
}
