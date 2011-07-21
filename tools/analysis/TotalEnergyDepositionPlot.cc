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

void TotalEnergyDepositionPlot::processEvent(const QVector<Hit*>&, const Particle* const particle, const SimpleEvent* const event)
{
  const TRDReconstruction* trdReconst = particle->trdReconstruction();
  if (!(trdReconst->flags() & TRDReconstruction::GoodTRDEvent))
    return;

  QList<double> valuesToFill;

  if (TRDReconstruction::calculateLengthInTube)
    for (int i = 0; i < 8; ++i)
      valuesToFill << trdReconst->energyDepositionOnTrackPerLengthForLayer(i);
  else
    for (int i = 0; i < 8; ++i)
      valuesToFill << trdReconst->energyDepositionOnTrackForLayer(i);


  double signalSum = 0.;
  for (QList<double>::const_iterator it = valuesToFill.constBegin(); it != valuesToFill.constEnd(); ++it)
    signalSum += *it;

  histogram()->Fill(signalSum);
}
