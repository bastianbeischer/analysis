#include "TRDDistanceInTube.hh"

#include <TH1D.h>

#include "Cluster.hh"
#include "Track.hh"
#include "TrackInformation.hh"
#include "TRDCalculations.hh"

TRDDistanceInTube::TRDDistanceInTube(AnalysisPlot::Topic topic) :
  AnalysisPlot(topic),
  H1DPlot()
{
  setTitle("TRD distance in tube");
  TH1D* histogram = new TH1D(qPrintable(title()), qPrintable(title() + ";distance / mm; entries"), 100, 0, 12);
  histogram->SetStats(true);
  addHistogram(histogram);
}

TRDDistanceInTube::~TRDDistanceInTube()
{
}

void TRDDistanceInTube::processEvent(const QVector<Hit*>& hits, Track* track, SimpleEvent* /*event*/)
{
  //check if everything worked and a track has been fit
  if (!track || !track->fitGood())
    return;

  //check if all tracker layers have a hit
  TrackInformation::Flags flags = track->information()->flags();
  if (!(flags & TrackInformation::AllTrackerLayers))
    return;

  //TODO: check for off track hits ?!?
  unsigned int nTrdHits = 0;
  const QVector<Hit*>::const_iterator hitsEnd = hits.end();
  for (QVector<Hit*>::const_iterator it = hits.begin(); it != hitsEnd; ++it) {
    if ((*it)->type() == Hit::trd)
      nTrdHits++;
  }

  if (nTrdHits < 6)
    return;

  for (QVector<Hit*>::const_iterator it = hits.begin(); it != hitsEnd; ++it) {
    Cluster* cluster = static_cast<Cluster*>(*it);
    if (cluster->type() != Hit::trd) 
      continue;
    
    std::vector<Hit*>& subHits = cluster->hits();
    const std::vector<Hit*>::const_iterator subHitsEndIt = subHits.end();
    for (std::vector<Hit*>::const_iterator it = subHits.begin(); it != subHitsEndIt; ++it) {
      Hit* subHit = *it;
      double distanceInTube = TRDCalculations::distanceOnTrackThroughTRDTube(subHit, track);
      if(distanceInTube > 0)
        histogram(0)->Fill(distanceInTube);
    }
  }
}

void TRDDistanceInTube::finalize()
{
}
