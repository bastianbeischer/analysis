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
  , m_layerEnergyDepositionOnTrackPerMinLength(8, 0.)
{
}

void TRDReconstruction::reset()
{
  m_allHits.clear();
  m_allClusters.clear();
  m_allHitsOnTrack.clear();
  m_allClustersOnTrack.clear();
  m_allHitsOnTrackAndPierced.clear();
  m_allClustersOnTrackAndPierced.clear();
  m_layerAllHits.clear();
  m_layerAllClusters.clear();
  m_layerAllHitsOnTrack.clear();
  m_layerAllHitsOnTrackAndPierced.clear();
  m_layerAllClustersOnTrack.clear();
  m_layerAllClustersOnTrackAndPierced.clear();
  //now set all values per layer to zero
  m_layerEnergyDeposition.fill(0.);
  m_layerEnergyDepositionOnTrack.fill(0.);
  m_layerEnergyDepositionOnTrackAndPierced.fill(0.);
  m_layerLengthThroughTube.fill(0.);
  m_layerEnergyDepositionOnTrackPerLength.fill(0.);
  m_layerEnergyDepositionOnTrackPerMinLength.fill(0.);
}

void TRDReconstruction::reconstructTRD(SimpleEvent* event, Track* globalTrack)
{
  reset();
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
        //m_layerEnergyDepositionOnTrack[trdLayer] += cluster->signalHeight();
      }
    }

    bool oneSubHitPierced = false;
    for (QVector<Hit*>::const_iterator it = hits.constBegin(); it != hits.constEnd(); ++it) {
      Hit* subHit = *it;
      m_allHits << subHit;
      m_layerAllHits.insert(trdLayer, subHit);
      m_layerEnergyDeposition[trdLayer] += subHit->signalHeight();
      if (globalTrack && (globalTrack->hits().contains(cluster) || globalTrack->hits().contains(subHit))) {
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

  //calculate the energy deposition per layer normalized to the length un tub, but use total energy on layer on track and use a minimum length of 1 mm
  for (int i = 0; i < 8; ++i)
    m_layerEnergyDepositionOnTrackPerMinLength[i] = m_layerEnergyDepositionOnTrack.at(i) / qMax(1.0 /*mm*/, m_layerLengthThroughTube.at(i));
}


int TRDReconstruction::getNoOfLayersWithEnergyDeposition() const
{
  int count = 0;
  for (QVector<double>::const_iterator it = m_layerEnergyDeposition.constBegin(); it != m_layerEnergyDeposition.constEnd(); ++it)
    if (*it > 0)
      ++count;
  return count;
}

int TRDReconstruction::getNoOfLayersWithEnergyDepositionOnTrack() const
{
  int count = 0;
  for (QVector<double>::const_iterator it = m_layerEnergyDepositionOnTrack.constBegin(); it != m_layerEnergyDepositionOnTrack.constEnd(); ++it)
    if (*it > 0)
      ++count;
  return count;
}

int TRDReconstruction::getNoOfLayersWithEnergyDepositionOnTrackAndPierced() const
{
  int count = 0;
  for (QVector<double>::const_iterator it = m_layerEnergyDepositionOnTrackAndPierced.constBegin(); it != m_layerEnergyDepositionOnTrackAndPierced.constEnd(); ++it)
    if (*it > 0)
      ++count;
  return count;
}

int TRDReconstruction::getNoOfLayersWithEnergyDepositionOnTrackPerLength() const
{
  int count = 0;
  for (QVector<double>::const_iterator it = m_layerEnergyDepositionOnTrackPerLength.constBegin(); it != m_layerEnergyDepositionOnTrackPerLength.constEnd(); ++it)
    if (*it > 0)
      ++count;
  return count;
}

int TRDReconstruction::getNoOfLayersWithEnergyDepositionOnTrackWithMinumLength() const
{
  int count = 0;
  for (QVector<double>::const_iterator it = m_layerEnergyDepositionOnTrackPerMinLength.constBegin(); it != m_layerEnergyDepositionOnTrackPerMinLength.constEnd(); ++it)
    if (*it > 0)
      ++count;
  return count;
}

