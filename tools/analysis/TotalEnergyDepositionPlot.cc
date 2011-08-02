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

  double signalSum = 0.;
  if (TRDReconstruction::s_calculateLengthInTube)
    for (int i = 0; i < 8; ++i)
      signalSum += trdReconst->energyDepositionForLayer(i).edepOnTrackPerLength;
  else
    for (int i = 0; i < 8; ++i)
      signalSum += trdReconst->energyDepositionForLayer(i).edepOnTrack;

  histogram()->Fill(signalSum);
}
