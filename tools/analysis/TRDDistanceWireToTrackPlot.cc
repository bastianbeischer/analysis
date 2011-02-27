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

void TRDDistanceWireToTrackPlot::processEvent(const QVector<Hit*>& /*hits*/, Track* track, SimpleEvent* /*event*/)
{
  //check if everything worked and a track has been fit
  if (!track || !track->fitGood())
    return;

  //filter: only use events with 8 tracker hits:
  TrackInformation::Flags flags = track->information()->flags();
  if (!(flags & TrackInformation::AllTrackerLayers))
    return;

  //TODO: check for off track hits ?!?
  unsigned int nTrdHits = 0;
  const QVector<Hit*>::const_iterator hitsEnd = track->hits().end();
  for (QVector<Hit*>::const_iterator it = track->hits().begin(); it != hitsEnd; ++it) {
    Hit* hit = *it;
    if (hit->type() == Hit::trd) {
      Cluster* cluster = static_cast<Cluster*>(hit);
      //check if event contains trd clusters with more than 2 sub hits
      if (cluster->hits().size() > 2)
        return;

      nTrdHits++;
    }
  }

  if (nTrdHits < 6)
    return;

  for (QVector<Hit*>::const_iterator it = track->hits().begin(); it != hitsEnd; ++it) {
    Cluster* cluster = static_cast<Cluster*>(*it);
    if (cluster->type() == Hit::trd) {
      double distanceWireToTrack = TRDCalculations::distanceTrackToWire(cluster, track);
      histogram(0)->Fill(distanceWireToTrack);
    }
  }
}

void TRDDistanceWireToTrackPlot::finalize()
{
}
