#include "ProtonRejector.hh"

#include <QVector>

#include "SimpleEvent.hh"
#include "Hit.hh"
#include "Cluster.hh"
#include "Track.hh"
#include "TrackInformation.hh"
#include "TRDCalculations.hh"



ProtonRejector::ProtonRejector()
{
}



bool ProtonRejector::isProton(const QVector<Hit*>& hits, const Track* track, const SimpleEvent* /*event*/, bool &ok){
  //check if everything worked and a track has been fit
  if (!track || !track->fitGood()){
    ok = false;
    return true;
  }

  //check if all tracker layers have a hit
  TrackInformation::Flags flags = track->information()->flags();
  if (!(flags & TrackInformation::AllTrackerLayers)){
    ok = false;
    return true;
  }

  //check if track was inside of magnet
  if (!(flags & TrackInformation::InsideMagnet)){
    ok = false;
    return true;
  }

  //get the reconstructed momentum
  double p = track->p(); //GeV

  //only use following momenta
  if(p < -10 || p > 10){
    ok = false;
    return true;
  }

  //loop over all hits and count tracker hits
  //also find all clusters on track
  QVector<Hit*> trdClusterHitsOnTrack;


  //TODO: check for off track hits ?!?
  foreach(Hit* clusterHit, hits){
    if (clusterHit->type() == Hit::trd)
      trdClusterHitsOnTrack.push_back(clusterHit);
  }


  //filter: only use events with 6 trd hits
  if (trdClusterHitsOnTrack.size() != 8){
    ok = false;
    return true;
  }



  QVector<double> dEdxValues;

  foreach(Hit* clusterHit, trdClusterHitsOnTrack){
    Cluster* cluster = static_cast<Cluster*>(clusterHit);
    foreach(Hit* hit, cluster->hits()){
      double distanceInTube = TRDCalculations::distanceOnTrackThroughTRDTube(hit, track);
      if(distanceInTube > 0)
        dEdxValues <<  (hit->signalHeight() / distanceInTube) ;
    }
  }

  if (dEdxValues.size() != 8){
    ok = false;
    return true;
  }

  //can be reconstructed
  ok = true;

  qSort(dEdxValues) ;

  if (dEdxValues.at(4) > 5) {
    return false;
  } else {
    return true;
  }

}
