#include "TRDDistanceWireToTrackPlot.hh"

#include <math.h>

#include <TH1D.h>

#include "Cluster.hh"
#include "SimpleEvent.hh"
#include "Constants.hh"
#include "Particle.hh"
#include "Track.hh"
#include "ParticleInformation.hh"
#include "TRDReconstruction.hh"

TRDDistanceWireToTrackPlot::TRDDistanceWireToTrackPlot(AnalysisTopic topic) :
  AnalysisPlot(topic),
  H1DPlot()
{
  setTitle("TRD distance of track to wire");
  setAxisTitle("distance track to wire / mm", "entries");
  TH1D* histogram = new TH1D(qPrintable(title()), "", 100, -15, 15);
  histogram->SetStats(true);
  histogram->Sumw2();
  addHistogram(histogram);
}

TRDDistanceWireToTrackPlot::~TRDDistanceWireToTrackPlot()
{
}

void TRDDistanceWireToTrackPlot::processEvent(const QVector<Hit*>&, const Particle* const particle, const SimpleEvent*)
{
  const TRDReconstruction* trdReconst = particle->trdReconstruction();
  if (!(trdReconst->flags() & TRDReconstruction::GoodTRDEvent))
    return;

  for (QVector<const Hit*>::const_iterator it = trdReconst->allHitsOnTrack().constBegin(); it != trdReconst->allHitsOnTrack().constEnd(); ++it)
    histogram()->Fill(TRDReconstruction::distanceTrackToWire(*it, particle->track()));
}
