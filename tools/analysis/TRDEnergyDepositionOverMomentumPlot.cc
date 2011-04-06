#include "TRDEnergyDepositionOverMomentumPlot.hh"

#include <QDebug>

#include "math.h"

#include <TH2D.h>
#include <TAxis.h>

#include "Cluster.hh"
#include "Track.hh"
#include "TrackInformation.hh"
#include "TRDCalculations.hh"

TRDEnergyDepositionOverMomentumPlot::TRDEnergyDepositionOverMomentumPlot(AnalysisPlot::Topic topic) :
  AnalysisPlot(topic),
  H2DPlot()
{
  setTitle("TRD energy deposition over momentum");
  //  setMultiGraphTitle(qPrintable(title() + ";rigidity / GV; energy deposition / (ADCCounts per mm per tube "));
  TH2D* histo = new TH2D(qPrintable(title()), qPrintable(title()), 200, -10, 10, 200, 0, 60);
  setAxisTitle("R / GV", "dE/dx", "");
  addHistogram(histo);
}

TRDEnergyDepositionOverMomentumPlot::~TRDEnergyDepositionOverMomentumPlot()
{
}

void TRDEnergyDepositionOverMomentumPlot::processEvent(const QVector<Hit*>& /*hits*/,Track* track, SimpleEvent* /*event*/)
{
  //check if everything worked and a track has been fit
  if (!track || !track->fitGood())
    return;

  //check if all tracker layers have a hit
  TrackInformation::Flags flags = track->information()->flags();
  if (!(flags & TrackInformation::AllTrackerLayers))
    return;

  //check if track was inside of magnet
  if (!(flags & TrackInformation::InsideMagnet))
    return;

  //get the reconstructed momentum
  double rigidity = track->rigidity(); //GeV

  //only use following momenta
  if(rigidity < -10 || rigidity > 10)
    return;

  //loop over all hits and count tracker hits
  //also find all clusters on track
  QVector<Hit*> trdClusterHitsOnTrack;


  //TODO: check for off track hits ?!?
  unsigned int nTrdHits = 0;
  const QVector<Hit*>::const_iterator hitsEnd = track->hits().end();
  for (QVector<Hit*>::const_iterator it = track->hits().begin(); it != hitsEnd; ++it) {
    if ((*it)->type() == Hit::trd)
      nTrdHits++;
  }

  if (nTrdHits < 6)
    return;

  QVector<double> energyDepPerTubePerDistance;

  for (QVector<Hit*>::const_iterator it = track->hits().begin(); it != hitsEnd; ++it) {
    Cluster* cluster = static_cast<Cluster*>(*it);
    if (cluster->type() != Hit::trd) 
      continue;
    
    std::vector<Hit*>& subHits = cluster->hits();
    const std::vector<Hit*>::const_iterator subHitsEndIt = subHits.end();
    for (std::vector<Hit*>::const_iterator it = subHits.begin(); it != subHitsEndIt; ++it) {
      Hit* subHit = *it;
      double distanceInTube = TRDCalculations::distanceOnTrackThroughTRDTube(subHit, track);
      if(distanceInTube > 0)
        energyDepPerTubePerDistance << (subHit->signalHeight() / distanceInTube);
    }
  }

  double meanEnergyDepPerTubePerDistance = 0.0;
  for(int i = 0; i < energyDepPerTubePerDistance.size(); ++i)
    meanEnergyDepPerTubePerDistance += energyDepPerTubePerDistance[i];

  meanEnergyDepPerTubePerDistance /= energyDepPerTubePerDistance.size();

  //qDebug() << "mean of " << energyDepPerTubePerDistance << " is " <<  meanEnergyDepPerTubePerDistance;
  histogram()->Fill(rigidity, meanEnergyDepPerTubePerDistance);
}

void TRDEnergyDepositionOverMomentumPlot::finalize()
{
}
