#include "ZenithDistributionPlot.hh"

#include <TH1D.h>
#include <TLatex.h>

#include "Hit.hh"
#include "ParticleInformation.hh"
#include "Particle.hh"
#include "Track.hh"
#include "SimpleEvent.hh"

#include <iostream>
#include <cmath>

ZenithDistributionPlot::ZenithDistributionPlot() :
  AnalysisPlot(AnalysisPlot::MomentumReconstruction),
  H1DPlot()
{
  setTitle("Zenith distribution");
  int nBins = 200;
  double xMin = 0;
  double xMax = 1;
  TH1D* histogram = new TH1D(qPrintable(title()), "", nBins, xMin, xMax);
  histogram->Sumw2();
  setAxisTitle("cos(zenith)", "entries");
  addHistogram(histogram);
  addLatex(RootPlot::newLatex(.15, .85));
}

ZenithDistributionPlot::~ZenithDistributionPlot()
{
}

void ZenithDistributionPlot::processEvent(const QVector<Hit*>&, const Particle* const particle, const SimpleEvent* const)
{
  const Track* track = particle->track();

  if (!track || !track->fitGood())
    return;

  ParticleInformation::Flags flags = particle->information()->flags();
  if ( !(flags & ParticleInformation::AllTrackerLayers) || !(flags & ParticleInformation::InsideMagnet) || (flags & ParticleInformation::Albedo) )
    return;
  
  double zenith = track->upperZenith();

  histogram()->Fill(cos(zenith));
}

void ZenithDistributionPlot::update()
{

}
