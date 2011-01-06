#include "TRDDistanceWireToTrackPlot.hh"

#include <math.h>

#include <TH1D.h>

#include "Cluster.hh"
#include "Constants.hh"
#include "Track.hh"
#include "TrackInformation.hh"
#include "TRDCalculations.hh"

TRDDistanceWireToTrackPlot::TRDDistanceWireToTrackPlot(AnalysisPlot::Topic topic) :
  AnalysisPlot(topic),
  H1DPlot()
{
  setTitle("TRD distance of cluster to wire");
  TH1D* histogram = new TH1D(qPrintable(title()), qPrintable(title() + ";distance / mm; entries"), 100, -15, 15);
  histogram->SetStats(true);
  addHistogram(histogram);
}

TRDDistanceWireToTrackPlot::~TRDDistanceWireToTrackPlot()
{
}

void TRDDistanceWireToTrackPlot::processEvent(const QVector<Hit*>& hits,Track* track, SimpleEvent* /*event*/)
{
  //check if everything worked and a track has been fit
  if (!track || !track->fitGood())
    return;

  //filter: only use events with 8 tracker hits:
  TrackInformation::Flags flags = track->information()->flags();
  if (!(flags & TrackInformation::AllTrackerLayers))
    return;

  //loop over all hits and count tracker hits
  //also find all clusters on track
  QVector<Cluster*> trdClustersOnTrack;

  //TODO: check for off track hits, atm this is Bastians criteria for on track
  foreach(Hit* clusterHit, hits){
    if (clusterHit->type() == Hit::trd){
      Cluster* cluster = static_cast<Cluster*>(clusterHit);
      //check if event contains trd clusters with more than 2 sub hits
      if (cluster->hits().size() > 2)
        return;

      trdClustersOnTrack << cluster;
    }
  }

  // cut on number of trd hits
  if (trdClustersOnTrack.size() < 6)
    return;

  foreach(Cluster* cluster, trdClustersOnTrack){
    //foreach(Hit* hit, cluster->hits()){
      double distanceWireToTrack = TRDCalculations::distanceTrackToWire(cluster, track);
      //if(fabs(distanceWireToTrack) > 10)

        histogram(0)->Fill(distanceWireToTrack);
    //}
  }
}

void TRDDistanceWireToTrackPlot::finalize()
{
}
