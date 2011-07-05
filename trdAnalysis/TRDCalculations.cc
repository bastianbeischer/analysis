#include "TRDCalculations.hh"

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

const bool TRDCalculations::calculateLengthInTube = true;
const int TRDCalculations::minTRDLayerCut = 6;
const int TRDCalculations::spectrumDefaultBins = 100;

TRDCalculations::TRDCalculations()
{
}


unsigned short TRDCalculations::TRDLayerNo(const Hit* hit)
{
  return TRDLayerNo(hit->detId());
}

unsigned short TRDCalculations::TRDLayerNo(const unsigned int detID)
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


double TRDCalculations::distanceTrackToWire(const Hit* hit, const Track* track)
{
  //check if trd
  if(hit->type() != Hit::trd)
    return 100.;

  TVector3 trdChanPos = hit->position();
  QVector2D trdChanPos2D(trdChanPos.x(),trdChanPos.z());

  return TRDCalculations::distanceTrackToWire(trdChanPos2D, track);
}


double TRDCalculations::distanceTrackToWire(const QVector2D& trdChanPos2D, const Track* track)
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

double TRDCalculations::distanceOnTrackThroughTRDTube(const Hit* hit, const Track* track)
{
  double distanceWireToTrack = fabs(TRDCalculations::distanceTrackToWire(hit,track));

  if(distanceWireToTrack >= Constants::TRDTubeRadius)
    return 0.0;

  double distanceInTube2D = 2.0* sqrt( Constants::TRDTubeRadius * Constants::TRDTubeRadius - distanceWireToTrack * distanceWireToTrack);

  //find y slope:
  double slopeYinTRD = track->slopeY(hit->position().z());

  double distanceInTube3D = distanceInTube2D * sqrt(slopeYinTRD*slopeYinTRD + 1.0) ;

  return distanceInTube3D;
}

bool TRDCalculations::globalTRDCuts(const QVector<Hit*>&, const Particle* particle, const SimpleEvent* event)
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
  unsigned int nTrdHitsOnTrack = 0;
  unsigned int nTotalTRDHits = 0;
  for (QVector<Hit*>::const_iterator it = track->hits().begin(); it != track->hits().end(); ++it) {
    if ((*it)->type() == Hit::trd)
      ++nTotalTRDHits;
    if (track->hits().contains(*it))
      ++nTrdHitsOnTrack;
  }

  //trd layer cut

  if (nTrdHitsOnTrack < TRDCalculations::minTRDLayerCut)
    return false;

  if (nTotalTRDHits-2 > nTrdHitsOnTrack)
    return false;

  //passed all cuts
  return true;
}
