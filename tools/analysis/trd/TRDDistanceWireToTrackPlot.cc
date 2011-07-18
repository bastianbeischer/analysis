#include "TRDDistanceWireToTrackPlot.hh"

#include <math.h>

#include <TH1D.h>

#include "Cluster.hh"
#include "SimpleEvent.hh"
#include "Constants.hh"
#include "Particle.hh"
#include "Track.hh"
#include "ParticleInformation.hh"
#include "TRDCalculations.hh"

TRDDistanceWireToTrackPlot::TRDDistanceWireToTrackPlot(AnalysisPlot::Topic topic) :
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

void TRDDistanceWireToTrackPlot::processEvent(const QVector<Hit*>& hits, Particle* particle, SimpleEvent* event)
{
  if (!TRDCalculations::globalTRDCuts(hits, particle, event))
      return;

  //now get all relevant energy deposition for this specific plot and all length
  QList<double> distanceList;

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
      double distancefromWire = TRDCalculations::distanceTrackToWire(subHit, track);
      distanceList << distancefromWire;
    } // subhits in cluster
  } // all hits

  if (distanceList.size() >= TRDCalculations::minTRDLayerCut) {
  foreach(double distance, distanceList)
    histogram()->Fill(distance);
  }
}
