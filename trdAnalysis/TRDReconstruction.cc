#include "TRDReconstruction.hh"

#include "SimpleEvent.hh"
#include "Track.hh"
#include "TRDCalculations.hh"
#include "Cluster.hh"


TRDReconstruction::TRDReconstruction()
  : m_layerEnergyDeposition(8, 0.)
  , m_layerEnergyDepositionOnTrack(8, 0.)
  , m_layerEnergyDepositionOnTrackAndPierced(8, 0.)
  , m_layerLengthThroughTube(8, 0.)
  , m_layerEnergyDepositionOnTrackPerLength(8, 0.)

{
}


void TRDReconstruction::reconstructTRD(SimpleEvent* event, Track* globalTrack)
{
  std::vector<Hit*>::const_iterator endIt = event->hits().end();
  for (std::vector<Hit*>::const_iterator eventHitIt = event->hits().begin(); eventHitIt != endIt; ++eventHitIt) {
    Hit* hit = *eventHitIt;
    if (hit->type() != Hit::trd)
      continue;
    QVector<Hit*> hits;
    Cluster* cluster = static_cast<Cluster*>(hit);
    if (cluster) {
      std::vector<Hit*>& subHits = cluster->hits();
      const std::vector<Hit*>::const_iterator subHitsEndIt = subHits.end();
      for (std::vector<Hit*>::const_iterator it = subHits.begin(); it != subHitsEndIt; ++it)
        hits << *it;
    } else {
      hits << hit;
    }
    int trdLayer = TRDCalculations::TRDLayerNo(hit);
    //now we have one raw hit or all subhits  of a cluster in the list.

    if (cluster) {
      m_allClusters << cluster;
      m_layerAllClusters.insert(trdLayer, cluster);
      if (globalTrack && globalTrack->hits().contains(cluster)) {
        m_allClustersOnTrack << cluster;
        m_layerAllClustersOnTrack.insert(trdLayer, cluster);
        m_layerEnergyDepositionOnTrack[trdLayer] += cluster->signalHeight();
      }
    }

    bool oneSubHitPierced = false;
    for (QVector<Hit*>::const_iterator it = hits.constBegin(); it != hits.constEnd(); ++it) {
      Hit* subHit = *it;
      m_allHits << subHit;
      m_layerAllHits.insert(trdLayer, subHit);
      m_layerEnergyDeposition[trdLayer] += subHit->signalHeight();
      if (globalTrack && globalTrack->hits().contains(cluster)) {
        m_allHitsOnTrack << subHit;
        m_layerAllHitsOnTrack.insert(trdLayer, subHit);
        m_layerEnergyDepositionOnTrack[trdLayer] += subHit->signalHeight();
      }
      double lengthInTube = TRDCalculations::distanceOnTrackThroughTRDTube(subHit, globalTrack);
      if (lengthInTube > 0.) {
        oneSubHitPierced = true;
        m_layerLengthThroughTube[trdLayer] += lengthInTube;
        m_allHitsOnTrackAndPierced << subHit;
        m_layerAllHitsOnTrackAndPierced.insert(trdLayer, subHit);
        m_layerEnergyDepositionOnTrackAndPierced[trdLayer] += subHit->signalHeight();
        m_layerEnergyDepositionOnTrackPerLength[trdLayer] += (subHit->signalHeight() / lengthInTube);
      }
    }

    if (oneSubHitPierced) {
      m_allClustersOnTrackAndPierced << cluster;
      m_layerAllClustersOnTrackAndPierced.insert(trdLayer, cluster);
    }

  } // all Clusters/Hits
}
