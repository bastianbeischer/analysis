#include "TRDClustersOnTrackPlot.hh"

#include <TH1D.h>

#include "Hit.hh"
#include "Cluster.hh"
#include "SimpleEvent.hh"
#include "Particle.hh"
#include "Track.hh"
#include "ParticleInformation.hh"

TRDClustersOnTrackPlot::TRDClustersOnTrackPlot(AnalysisPlot::Topic topic) :
  AnalysisPlot(topic),
  H1DPlot()
{
  setTitle("TRD clusters on track");
  setAxisTitle("number of trd clusters on track", "entries");
  TH1D* histogram = new TH1D(qPrintable(title()), "", 11, -0.5, 10.5);
  histogram->SetStats(true);
  histogram->Sumw2();
  addHistogram(histogram);
}

TRDClustersOnTrackPlot::~TRDClustersOnTrackPlot()
{
}

void TRDClustersOnTrackPlot::processEvent(const QVector<Hit*>& /*hits*/,Particle* particle ,SimpleEvent*)
{
  const Track* track = particle->track();

  //check if everything worked and a track has been fit
  if (!track || !track->fitGood())
    return;

  if (particle->information()->flags() & ParticleInformation::Chi2Good)
    return;

  int nTRDHits = 0;
  QVector<Hit*>::const_iterator endIt = track->hits().end();
  for (QVector<Hit*>::const_iterator it = track->hits().begin(); it != endIt; ++it) {
    if ((*it)->type() == Hit::trd)
      ++nTRDHits;
  }

  histogram(0)->Fill(nTRDHits);
}

