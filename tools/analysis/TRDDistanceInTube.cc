#include "TRDDistanceInTube.hh"

#include <TH1D.h>

#include "Cluster.hh"
#include "SimpleEvent.hh"
#include "Particle.hh"
#include "Track.hh"
#include "ParticleInformation.hh"
#include "TRDReconstruction.hh"

TRDDistanceInTube::TRDDistanceInTube(AnalysisPlot::Topic topic) :
  AnalysisPlot(topic),
  H1DPlot()
{
  setTitle("TRD length in tube");
  setAxisTitle("length through tube / mm", "entries");
  TH1D* histogram = new TH1D(qPrintable(title()), "", 100, 0, 12);
  histogram->SetStats(true);
  histogram->Sumw2();
  addHistogram(histogram);
}

TRDDistanceInTube::~TRDDistanceInTube()
{
}

void TRDDistanceInTube::processEvent(const QVector<Hit*>& /*hits*/, const Particle* const particle, const SimpleEvent* const event)
{
  const TRDReconstruction* trdReconst = particle->trdReconstruction();
  if (!(trdReconst->flags() & TRDReconstruction::GoodTRDEvent))
    return;

  for (int i = 0; i < 8; ++i)
    histogram()->Fill(trdReconst->lengthThroughTubeForLayer(i));
}
