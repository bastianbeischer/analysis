#include "TRDEnergyDepositionOverMomentumPlot.hh"

#include <QDebug>

#include "math.h"

#include <TGraph.h>
#include <TAxis.h>
#include <CLHEP/Units/SystemOfUnits.h>

#include "Cluster.hh"
#include "Track.hh"
#include "TrackInformation.hh"
#include "TRDCalculations.hh"

TRDEnergyDepositionOverMomentumPlot::TRDEnergyDepositionOverMomentumPlot(AnalysisPlot::Topic topic) :
  AnalysisPlot(topic),
  GraphPlot()
{
  setTitle("TRD energy deposition over momentum");
  setMultiGraphTitle(qPrintable(title() + ";momentum / GeV; energy deposition / (ADCCounts per mm per tube "));

  TGraph* graph = new TGraph();
  graph->SetMarkerStyle(1);
  addGraph(graph, "P");

  graph->GetXaxis()->SetRangeUser(-10,10);
}

TRDEnergyDepositionOverMomentumPlot::~TRDEnergyDepositionOverMomentumPlot()
{
}

void TRDEnergyDepositionOverMomentumPlot::processEvent(const QVector<Hit*>& hits,Track* track, SimpleEvent* /*event*/)
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
  double p = track->p(); //GeV

  //only use following momenta
  if(p < -10 || p > 10)
    return;

  //loop over all hits and count tracker hits
  //also find all clusters on track
  QVector<Hit*> trdClusterHitsOnTrack;


  //TODO: check for off track hits ?!?
  foreach(Hit* clusterHit, hits){
    if (clusterHit->type() == Hit::trd)
      trdClusterHitsOnTrack.push_back(clusterHit);
  }


  //filter: only use events with 6 trd hits
  if (trdClusterHitsOnTrack.size() < 6)
    return;

  QVector<double> energyDepPerTubePerDistance;

  foreach(Hit* clusterHit, trdClusterHitsOnTrack){
    Cluster* cluster = static_cast<Cluster*>(clusterHit);
    foreach(Hit* hit, cluster->hits()){
      double distanceInTube = TRDCalculations::distanceOnTrackThroughTRDTube(hit, track);
      if(distanceInTube > 0)
        energyDepPerTubePerDistance << (hit->signalHeight() / distanceInTube);
    }
  }

  double meanEnergyDepPerTubePerDistance = 0.0;
  for(int i = 0; i < energyDepPerTubePerDistance.size(); ++i)
    meanEnergyDepPerTubePerDistance += energyDepPerTubePerDistance[i];

  meanEnergyDepPerTubePerDistance /= energyDepPerTubePerDistance.size();

  //qDebug() << "mean of " << energyDepPerTubePerDistance << " is " <<  meanEnergyDepPerTubePerDistance;
  //add a point

  QMutexLocker locker(&m_mutex);
  //  for(int i = 0; i < energyDepPerTubePerDistance.size(); ++i){
  //      quint32 numberOfPointsInGraph = graph(0)->GetN();
  //      graph(0)->SetPoint(numberOfPointsInGraph, p, energyDepPerTubePerDistance[i]);
  //  }

  quint32 numberOfPointsInGraph = graph(0)->GetN();
  graph(0)->SetPoint(numberOfPointsInGraph, p, meanEnergyDepPerTubePerDistance);
}

void TRDEnergyDepositionOverMomentumPlot::finalize()
{
}
