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
#include <Settings.hh>
#include <SettingsManager.hh>

#include <TVector3.h>
#include <QVector2D>
#include <QString>

//#############################
//static members:
const bool TRDReconstruction::calculateLengthInTube = true;
const int TRDReconstruction::minLayerCut = 6;
const int TRDReconstruction::spectrumDefaultBins = 100;


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
    int trdLayer = TRDReconstruction::TRDLayerNo(hit);
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
      double lengthInTube = TRDReconstruction::distanceOnTrackThroughTRDTube(subHit, globalTrack);
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


//#############################
//static functions:

int TRDReconstruction::TRDLayerNo(const Hit* hit)
{
  return TRDLayerNo(hit->detId());
}

int TRDReconstruction::TRDLayerNo(const unsigned int detID)
{
  //returns the trdlayer number counted from top to bottom 0-7

  unsigned short trdQuarterID = detID & 0xFF00;
  unsigned short trdLayerInQuarter = (detID & 0x00F0) >> 4;
  switch (trdQuarterID) {
    case 0x3400: case 0x3500: return 7-trdLayerInQuarter; break;
    case 0x3200: case 0x3600: return 3-trdLayerInQuarter; break;
    default: return 99;
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

bool TRDReconstruction::globalTRDCuts(const QVector<Hit*>&, const Particle* particle, const SimpleEvent* event)
{
  const Track* track = particle->track();
  const ParticleInformation::Flags pFlags = particle->information()->flags();

  //check if everything worked and a track has been fit
  if (!track || !track->fitGood())
    return false;

  if (pFlags & ParticleInformation::Chi2Good)
    return false;

  //get settings if present
  const Settings* settings = SettingsManager::instance()->settingsForEvent(event);

  //check if magnet was installed, if it was or no information was found, check if the particle went through the inner magnet:
  if (!(settings && !(settings->magnet()))){
    //check if track was inside of magnet
    if (!(pFlags & ParticleInformation::InsideMagnet)  )
      return false;
  }

  //count trd hits which belong to the track and those of the event
  int nTrdHitsOnTrack = 0;
  int nTotalTRDHits = 0;
  for (QVector<Hit*>::const_iterator it = track->hits().begin(); it != track->hits().end(); ++it) {
    if ((*it)->type() == Hit::trd)
      ++nTotalTRDHits;
    if (track->hits().contains(*it))
      ++nTrdHitsOnTrack;
  }

  //trd layer cut

  if (nTrdHitsOnTrack < TRDReconstruction::minTRDLayerCut)
    return false;

  if (nTotalTRDHits-2 > nTrdHitsOnTrack)
    return false;

  //passed all cuts
  return true;
}


