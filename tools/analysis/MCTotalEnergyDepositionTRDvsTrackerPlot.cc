#include "MCTotalEnergyDepositionTRDvsTrackerPlot.hh"

#include <TPad.h>
#include <TMultiGraph.h>
#include <TGraph.h>
#include <TLegend.h>

#include "Track.hh"
#include "TrackInformation.hh"
#include "SimpleEvent.hh"
#include "Cluster.hh"
#include "Hit.hh"

#include "TRDCalculations.hh"
#include "RootStyle.hh"

MCTotalEnergyDepositionTRDvsTrackerPlot::MCTotalEnergyDepositionTRDvsTrackerPlot()
  : AnalysisPlot(AnalysisPlot::MonteCarlo)
  , GraphPlot()
  , m_colorCounter(0)
{
  setTitle("MC Mean dE_dx TRD vs. tracker");

  TLegend* legend = new TLegend(.72, .72, .98, .98);
  legend->SetFillColor(kWhite);
  legend->SetMargin(.7);
  addLegend(legend);
}

MCTotalEnergyDepositionTRDvsTrackerPlot::~MCTotalEnergyDepositionTRDvsTrackerPlot()
{
}

void MCTotalEnergyDepositionTRDvsTrackerPlot::processEvent(const QVector<Hit*>& hits, Track* track, SimpleEvent* event)
{
  //only accept mc events:
  if (event->contentType() != SimpleEvent::MonteCarlo)
    return;

  //check if everything worked and a track has been fit
  if (!track || !track->fitGood())
    return;

  if (track->chi2() / track->ndf() > 10)
    return;

  //check if track was inside of magnet
  if (!(track->information()->flags() & TrackInformation::InsideMagnet))
    return;


  /*
  //get the reconstructed momentum
  double p = track->p(); //GeV

  if( p < m_lowerMomentum || p > m_upperMomentum)
    return;

  if (track->beta() < 1.5 || track->beta() < 0)
    return;
  */

  double signalSumTRD = 0;
  double distanceSumTRD = 0;
  double signalSumTracker = 0;

  int trdCluster = 0;
  int trackerCluster = 0;

  const QVector<Hit*>::const_iterator endIt = hits.end();
  for (QVector<Hit*>::const_iterator it = hits.begin(); it != endIt; ++it) {
    Cluster* cluster = static_cast<Cluster*>(*it);
    if (cluster->type() == Hit::trd){
      std::vector<Hit*>& subHits = cluster->hits();
      std::vector<Hit*>::const_iterator subHitsEndIt = subHits.end();
      for (std::vector<Hit*>::const_iterator it = subHits.begin(); it != subHitsEndIt; ++it) {
        Hit* trdHit = *it;
        double distance = TRDCalculations::distanceOnTrackThroughTRDTube(trdHit, track);
        if (distance > 0) {
          distanceSumTRD += distance;
          signalSumTRD += cluster->signalHeight();
          trdCluster++;
        }
      }
    }else if ( cluster->type() == Hit::tracker ) {
      // find max strip
      unsigned short iMax = 0;
      double max = 0.;
      for(unsigned short i = 0; i < cluster->hits().size(); ++i) {
        double amplitude = cluster->hits().at(i)->signalHeight();
        if (amplitude > max) {
          max = amplitude;
          iMax = i;
        }
      }
      trackerCluster++;
      //cluster amplitude - max channel
      signalSumTracker += cluster->signalHeight() - max;
    }
  }

  //cut on trd cluster
  if (trdCluster < 5)
    return;

  //get graph:
  int pdgID = event->MCInformation()->primary()->pdgID;

  TGraph* graph = 0;

  QMutexLocker locker(&m_mutex);
  if (m_graphMap.contains(pdgID))
    graph = m_graphMap.value(pdgID);
  else
  {
    graph = new TGraph();
    graph->SetMarkerColor(RootStyle::rootColor(m_colorCounter++));
    graph->SetMarkerSize(0.3);
    m_graphMap.insert(pdgID, graph);
    addGraph(graph, "P");
    QString particleName = QString::number(pdgID);
    legend()->AddEntry(graph, qPrintable(particleName), "p");
  }

  int nPoints = graph->GetN();
  graph->SetPoint(nPoints, signalSumTracker / trackerCluster, signalSumTRD / distanceSumTRD);

}
