#include "TRDCalculations.hh"

#include <math.h>

#include <Hit.hh>
#include <Track.hh>
#include <StraightLine.hh>
#include <BrokenLine.hh>
#include <CenteredBrokenLine.hh>
#include <Constants.hh>

#include <TVector3.h>

#include <QVector2D>


TRDCalculations::TRDCalculations()
{
}

double TRDCalculations::distanceTrackToWire(const Hit* hit, const Track* track)
{
  //check if trd
  if(hit->type() != Hit::trd)
    return 100.;

  TVector3 trdChanPos = hit->position();
  QVector2D trdChanPos2D(trdChanPos.x(),trdChanPos.z());

  TVector3 trackPosAtZOfChannel = track->position(trdChanPos.z());
  QVector2D trackPosAtZOfChannel2D(trackPosAtZOfChannel.x() , trackPosAtZOfChannel.z());


  //use lower slope, as we are in the trd, find correct track type:
  double slopeXinTRD = track->slopeX(trdChanPos.z());

  QVector2D trackNormal = QVector2D(1.0/slopeXinTRD, -1.0).normalized();

  QVector2D circleToTrackAtLevel = trackPosAtZOfChannel2D - trdChanPos2D;

  //qDebug("d on channel  level = %f mm", circleToTrackAtLevel.length());

  return QVector2D::dotProduct(trackNormal, circleToTrackAtLevel ) ;

}

double TRDCalculations::distanceOnTrackThroughTRDTube(const Hit* hit, const Track* track)
{
  double distanceWireToTrack = fabs(TRDCalculations::distanceTrackToWire(hit,track));

  if(distanceWireToTrack >= TRDTubeRadius)
    return 0.0;

  double distanceInTube2D = 2.0* sqrt( TRDTubeRadius * TRDTubeRadius - distanceWireToTrack * distanceWireToTrack);

  //find y slope:
  double slopeYinTRD = track->slopeY(hit->position().z());

  double distanceInTube3D = distanceInTube2D * sqrt(slopeYinTRD*slopeYinTRD + 1.0) ;

  return distanceInTube3D;
}
