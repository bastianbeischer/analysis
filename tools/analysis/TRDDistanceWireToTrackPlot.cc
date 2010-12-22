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
  setTitle("TRD distance track to wire");
  TH1D* histogram = new TH1D(qPrintable(title()), qPrintable(title() + ";distance / mm; entries"), 100, -3.5, 3.5);
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
  QVector<Hit*> trdClusterHitsOnTrack;

  //TODO: check for off track hits ?!?
  foreach(Hit* clusterHit, hits){
    if (clusterHit->type() == Hit::trd)
      trdClusterHitsOnTrack.push_back(clusterHit);
  }

  // cut on number of trd hits
  if (trdClusterHitsOnTrack.size() < 6)
    return;

  foreach(Hit* clusterHit, trdClusterHitsOnTrack){
    Cluster* cluster = static_cast<Cluster*>(clusterHit);
    foreach(Hit* hit, cluster->hits()){
      double distanceWireToTrack = TRDCalculations::distanceTrackToWire(hit, track);
      if(fabs(distanceWireToTrack)< TRDTubeRadius)
        histogram(0)->Fill(distanceWireToTrack);
    }
  }
}

void TRDDistanceWireToTrackPlot::finalize()
{
}
