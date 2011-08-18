#include "TRDReconstruction.hh"

#include "SimpleEvent.hh"
#include "Track.hh"
#include "Cluster.hh"

#include <math.h>

#include <Hit.hh>
#include <Track.hh>
#include <StraightLine.hh>
#include <SimpleEvent.hh>
#include <Particle.hh>
#include <ParticleInformation.hh>

#include <BrokenLine.hh>
#include <CenteredBrokenLine.hh>
#include <Constants.hh>

#include <TVector3.h>
#include <QVector2D>
#include <QString>

const bool TRDReconstruction::s_calculateLengthInTube = true;
const int TRDReconstruction::s_minLayerCut = 4;
const int TRDReconstruction::s_maxOffTrackCluster = 2;
const int TRDReconstruction::s_spectrumDefaultBins = 100;


TRDReconstruction::TRDReconstruction()
  : m_flags(None)
{
}

void TRDReconstruction::reset()
{
  m_flags = None;
  m_allHits.clear();
  m_allClusters.clear();
  m_allHitsOnTrack.clear();
  m_allClustersOnTrack.clear();
  m_allHitsOnTrackAndPierced.clear();
  m_allClustersOnTrackAndPierced.clear();
  for (int i = 0; i < 8; i++) {
    m_layerAllHits[i].clear();
    m_layerAllClusters[i].clear();
    m_layerAllHitsOnTrack[i].clear();
    m_layerAllHitsOnTrackAndPierced[i].clear();
    m_layerAllClustersOnTrack[i].clear();
    m_layerAllClustersOnTrackAndPierced[i].clear();
  }
  m_channelEdep.clear();
  m_moduleEdep.clear();

  for (int i = 0; i < 8; ++i) {
    m_layerEdep[i].edep = 0.;
    m_layerEdep[i].edepOnTrack = 0.;
    m_layerEdep[i].lengthThroughTube = 0.;
    m_layerEdep[i].edepOnTrackAndPierced = 0.;
    m_layerEdep[i].edepOnTrackPerLength = 0.;
    m_layerEdep[i].isOnTRack = false;
    m_layerEdep[i].isPierced = false;
  }
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
    int trdLayer = TRDReconstruction::TRDLayerNo(hit);
    //now we have the single hit or all subhits of a cluster in the list.

    //do the general sorting of the cluster, if presentm in the correct lists and layer
    if (cluster) {
      m_allClusters << cluster;
      m_layerAllClusters[trdLayer].append(cluster);
      if (globalTrack && globalTrack->fitGood() && globalTrack->hits().contains(cluster)) {
        m_allClustersOnTrack << cluster;
        m_layerAllClustersOnTrack[trdLayer].append(cluster);
      }
    }

    //now loop over all subhits:
    bool oneSubHitPierced = false;
    for (QVector<Hit*>::const_iterator it = hits.constBegin(); it != hits.constEnd(); ++it) {
      Hit* subHit = *it;
      double amplitude = subHit->signalHeight();
      unsigned short channelId = subHit->detId();
      unsigned short moduleId = channelId & 0xFFF0;
      m_allHits << subHit;
      m_layerAllHits[trdLayer] << subHit;
      m_layerEdep[trdLayer].edep += amplitude;
      m_channelEdep[channelId].edep += amplitude;
      m_moduleEdep[moduleId].edep += amplitude;

      //subit is on track or cluster of subhit is on track
      if (globalTrack && globalTrack->fitGood() && (globalTrack->hits().contains(cluster) || globalTrack->hits().contains(subHit))) {
        m_allHitsOnTrack << subHit;
        m_layerAllHitsOnTrack[trdLayer] << subHit;
        m_layerEdep[trdLayer].edepOnTrack += amplitude;
        m_channelEdep[channelId].edepOnTrack += amplitude;
        m_moduleEdep[moduleId].edepOnTrack += amplitude;
        m_layerEdep[trdLayer].isOnTRack = true; //dose it make sense for layers?
        m_channelEdep[channelId].isOnTRack = true;
        m_moduleEdep[moduleId].isOnTRack = true; //dose it make sense for modules?
      }

      //the subhit has been pierced by the global track
      double lengthInTube = TRDReconstruction::distanceOnTrackThroughTRDTube(subHit, globalTrack);
      if (lengthInTube > 0.) {
        oneSubHitPierced = true;
        m_allHitsOnTrackAndPierced << subHit;
        m_layerAllHitsOnTrackAndPierced[trdLayer] << subHit;
        m_layerEdep[trdLayer].lengthThroughTube += lengthInTube;
        m_channelEdep[channelId].lengthThroughTube += lengthInTube;
        m_moduleEdep[moduleId].lengthThroughTube += lengthInTube;
        m_layerEdep[trdLayer].edepOnTrackAndPierced += amplitude;
        m_channelEdep[channelId].edepOnTrackAndPierced += amplitude;
        m_moduleEdep[moduleId].edepOnTrackAndPierced += amplitude;
        m_layerEdep[trdLayer].edepOnTrackPerLength += (amplitude / lengthInTube);
        m_channelEdep[channelId].edepOnTrackPerLength += (amplitude / lengthInTube);
        m_moduleEdep[moduleId].edepOnTrackPerLength += (amplitude / lengthInTube);
        m_layerEdep[trdLayer].isPierced = true; //dose it make sense for layers?
        m_channelEdep[channelId].isPierced = true;
        m_moduleEdep[moduleId].isPierced = true; //dose it make sense for modules?
      }
    }

    //we can only check if subhits have been pierced, if this is true add the cluster to the pierced lists
    if (oneSubHitPierced) {
      m_allClustersOnTrackAndPierced << cluster;
      m_layerAllClustersOnTrackAndPierced[trdLayer].append(cluster);
    }

  } // all Clusters/Hits

  checkGoodTRDEvent(globalTrack);
}

void TRDReconstruction::checkGoodTRDEvent(const Track* track)
{
  int nLayersWithEnDepOnTrack = noOfLayersWithEnergyDepositionOnTrack();
  int nOffTrackCluster = noOfClusters() - noOfClustersOnTrack();

  bool layerCut = nLayersWithEnDepOnTrack >= s_minLayerCut;
  bool offTrackHitCut =nOffTrackCluster <= s_maxOffTrackCluster;
  bool trackCut = track && track->fitGood() && ((track->chi2() / track->ndf()) < 5);

  if (layerCut && offTrackHitCut && trackCut)
    m_flags |= GoodTRDEvent;
}

int TRDReconstruction::noOfLayersWithEnergyDeposition() const
{
  int count = 0;
  for (int i = 0; i < 8; ++i)
    if (m_layerEdep[i].edep > 0)
      ++count;
  return count;
}

int TRDReconstruction::noOfLayersWithEnergyDepositionOnTrack() const
{
  int count = 0;
  for (int i = 0; i < 8; ++i)
    if (m_layerEdep[i].isOnTRack)
      ++count;
  return count;
}

int TRDReconstruction::noOfLayersWithEnergyDepositionOnTrackAndPierced() const
{
  int count = 0;
  for (int i = 0; i < 8; ++i)
    if (m_layerEdep[i].isPierced)
      ++count;
  return count;
}


//#############################
//static functions:

unsigned char TRDReconstruction::TRDLayerNo(const Hit* hit)
{
  if (hit->type() != Hit::trd)
    Q_ASSERT(false);
  return TRDLayerNo(hit->detId());
}

unsigned char TRDReconstruction::TRDLayerNo(const unsigned short detID)
{
  //returns the trdlayer number counted from top to bottom 0-7

  unsigned short trdQuarterID = detID & 0xFF00;
  unsigned short trdLayerInQuarter = (detID & 0x00F0) >> 4;
  switch (trdQuarterID) {
    case 0x3400: case 0x3500: return 7-trdLayerInQuarter;
    case 0x3200: case 0x3600: return 3-trdLayerInQuarter;
    default: Q_ASSERT(false); return 0xFF;
  }
}


double TRDReconstruction::distanceTrackToWire(const Hit* hit, const Track* track)
{
  //check if trd
  if(hit->type() != Hit::trd)
    return 100.;

  TVector3 trdChanPos = hit->position();
  QVector2D trdChanPos2D(trdChanPos.x(),trdChanPos.z());

  return TRDReconstruction::distanceTrackToWire(trdChanPos2D, track);
}


double TRDReconstruction::distanceTrackToWire(const QVector2D& trdChanPos2D, const Track* track)
{
  //trdChanPos2D = (x coordinate, z coordinate)

  TVector3 trackPosAtZOfChannel = track->position(trdChanPos2D.y());
  QVector2D trackPosAtZOfChannel2D(trackPosAtZOfChannel.x() , trackPosAtZOfChannel.z());


  //use slope
  double slopeXinTRD = track->slopeX(trdChanPos2D.y());

  QVector2D trackNormal = QVector2D(1.0/slopeXinTRD, -1.0).normalized();

  QVector2D circleToTrackAtLevel = trackPosAtZOfChannel2D - trdChanPos2D;

  //qDebug("d on channel  level = %f mm", circleToTrackAtLevel.length());

  return QVector2D::dotProduct(trackNormal, circleToTrackAtLevel ) ;

}

double TRDReconstruction::distanceOnTrackThroughTRDTube(const Hit* hit, const Track* track)
{
  double distanceWireToTrack = fabs(TRDReconstruction::distanceTrackToWire(hit,track));

  if(distanceWireToTrack >= Constants::TRDTubeRadius)
    return 0.0;

  double distanceInTube2D = 2.0* sqrt( Constants::TRDTubeRadius * Constants::TRDTubeRadius - distanceWireToTrack * distanceWireToTrack);

  //find y slope:
  double slopeYinTRD = track->slopeY(hit->position().z());

  double distanceInTube3D = distanceInTube2D * sqrt(slopeYinTRD*slopeYinTRD + 1.0) ;

  return distanceInTube3D;
}

