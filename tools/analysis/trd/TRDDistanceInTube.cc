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

void TRDDistanceInTube::processEvent(const QVector<Hit*>& hits, Particle* particle, SimpleEvent* event)
{
  if (!TRDReconstruction::globalTRDCuts(hits, particle, event))
      return;

  //now get all relevant energy deposition for this specific plot and all length
  QList<double> lengthList;

  const Track* track = particle->track();
  for (QVector<Hit*>::const_iterator it = track->hits().begin(); it != track->hits().end(); ++it) {
    Hit* hit = *it;
    if (hit->type() != Hit::trd)
      continue;
    Cluster* cluster = static_cast<Cluster*>(hit);
    std::vector<Hit*>& subHits = cluster->hits();
    const std::vector<Hit*>::const_iterator subHitsEndIt = subHits.end();
    for (std::vector<Hit*>::const_iterator it = subHits.begin(); it != subHitsEndIt; ++it) {
      Hit* subHit = *it;
      double distanceInTube = TRDReconstruction::distanceOnTrackThroughTRDTube(subHit, track);
      lengthList << distanceInTube;
    } // subhits in cluster
  } // all hits

  if (lengthList.size() >= TRDReconstruction::minTRDLayerCut) {
    foreach(double length, lengthList)
      histogram()->Fill(length);
  }
}
