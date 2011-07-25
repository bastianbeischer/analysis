#include "TotalEnergyDepositionPlot.hh"

#include <TH1D.h>

#include "Particle.hh"
#include "Track.hh"
#include "ParticleInformation.hh"
#include "SimpleEvent.hh"
#include "Cluster.hh"
#include "Hit.hh"

#include "TRDReconstruction.hh"

TotalEnergyDepositionPlot::TotalEnergyDepositionPlot()
  : AnalysisPlot(AnalysisPlot::MiscellaneousTRD)
  , H1DPlot()
{

  setTitle("Total Signal Sum");

  TH1D* histogram = new TH1D(qPrintable(title()), qPrintable(title() + ";total signal sum;entries"), 100, 0, 40);
  addHistogram(histogram);
}

TotalEnergyDepositionPlot::~TotalEnergyDepositionPlot()
{
}

void TotalEnergyDepositionPlot::processEvent(const QVector<Hit*>&, const Particle* const particle, const SimpleEvent*)
{
  const TRDReconstruction* trdReconst = particle->trdReconstruction();
  if (!(trdReconst->flags() & TRDReconstruction::GoodTRDEvent))
    return;

  const QVector<double>& values = TRDReconstruction::s_calculateLengthInTube ?
        trdReconst->energyDepositionOnTrackPerLengthForLayers() : trdReconst->energyDepositionOnTrackForLayers();

  double signalSum = 0.;
  for (QVector<double>::const_iterator it = values.constBegin(); it != values.constEnd(); ++it)
    signalSum += *it;

  histogram()->Fill(signalSum);
}
