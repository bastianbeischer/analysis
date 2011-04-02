#include "TRDClustersOnTrackPlot.hh"

#include <TH1D.h>

#include "Hit.hh"
#include "Cluster.hh"
#include "SimpleEvent.hh"
#include "Track.hh"
#include "TrackInformation.hh"

TRDClustersOnTrackPlot::TRDClustersOnTrackPlot(AnalysisPlot::Topic topic) :
  AnalysisPlot(topic),
  H1DPlot()
{
  setTitle("TRD clusters on track");
  TH1D* histogram = new TH1D(qPrintable(title()), "", 11, -0.5, 10.5);
  histogram->SetStats(true);
  addHistogram(histogram);
  TH1D* histogramMC = new TH1D(qPrintable(title()+"MC"), "MC", 11, -0.5, 10.5);
  histogramMC->SetLineColor(kRed);
  addHistogram(histogramMC);
}

TRDClustersOnTrackPlot::~TRDClustersOnTrackPlot()
{
}

void TRDClustersOnTrackPlot::processEvent(const QVector<Hit*>& /*hits*/,Track* track ,SimpleEvent* event)
{

  //check if everything worked and a track has been fit
  if (!track || !track->fitGood())
    return;

  //check if all tracker layers have a hit
  TrackInformation::Flags flags = track->information()->flags();
  if (!(flags & TrackInformation::AllTrackerLayers))
    return;

  int nTRDHits = 0;
  QVector<Hit*>::const_iterator endIt = track->hits().end();
  for (QVector<Hit*>::const_iterator it = track->hits().begin(); it != endIt; ++it) {
    if ((*it)->type() == Hit::trd)
      ++nTRDHits;
  }

  if (event->contentType() == SimpleEvent::MonteCarlo)
    histogram(1)->Fill(nTRDHits);
  else
    histogram(0)->Fill(nTRDHits);


}

void TRDClustersOnTrackPlot::finalize()
{
}
