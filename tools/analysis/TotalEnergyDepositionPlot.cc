#include "TotalEnergyDepositionPlot.hh"

#include <TH1D.h>

#include "Track.hh"
#include "TrackInformation.hh"
#include "Cluster.hh"
#include "Hit.hh"

#include "TRDCalculations.hh"

TotalEnergyDepositionPlot::TotalEnergyDepositionPlot(double lowerMom, double upperMom) :
    AnalysisPlot(AnalysisPlot::SignalHeightTRD),
  H1DPlot(),
  m_lowerMomentum(lowerMom),
  m_upperMomentum(upperMom)
{

  setTitle(QString("Total Signal Sum (%1 GV to %2 GV)").arg(m_lowerMomentum).arg(m_upperMomentum));

  TH1D* histogram = new TH1D(qPrintable(title()), qPrintable(title() + ";dE/dx ;entries"), 100, 0, 30);
  addHistogram(histogram);
}

TotalEnergyDepositionPlot::~TotalEnergyDepositionPlot()
{
}

void TotalEnergyDepositionPlot::processEvent(const QVector<Hit*>& hits, Track* track, SimpleEvent*)
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

  if(p < m_lowerMomentum || p > m_upperMomentum)
    return;

  //loop over all hits and count tracker hits
  //also find all clusters on track
  QVector<Hit*> trdClusterHitsOnTrack;

  //TODO: check for off track hits ?!?
  foreach(Hit* clusterHit, hits){
    if (clusterHit->type() == Hit::trd)
      trdClusterHitsOnTrack.push_back(clusterHit);
  }

  if(trdClusterHitsOnTrack.size() != 8)
    return;


  double signalSum = 0;
  double distanceSum = 0;

  foreach(Hit* clusterHit, trdClusterHitsOnTrack){
    Cluster* cluster = static_cast<Cluster*>(clusterHit);
    foreach(Hit* hit, cluster->hits()){
      double distanceInTube = TRDCalculations::distanceOnTrackThroughTRDTube(hit, track);
      if(distanceInTube > 0){
        signalSum += hit->signalHeight() ;
        distanceSum += distanceInTube ;
      }
    }
  }



  histogram(0)->Fill(signalSum / distanceSum);

}
