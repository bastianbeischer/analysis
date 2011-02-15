#include "TotalEnergyDepositionPlot.hh"

#include <TH1D.h>

#include "Track.hh"
#include "TrackInformation.hh"
#include "SimpleEvent.hh"
#include "Cluster.hh"
#include "Hit.hh"

#include "TRDCalculations.hh"

TotalEnergyDepositionPlot::TotalEnergyDepositionPlot(double lowerMom, double upperMom) :
    AnalysisPlot(AnalysisPlot::MiscellaneousTRD),
  H1DPlot(),
  m_lowerMomentum(lowerMom),
  m_upperMomentum(upperMom)
{

  setTitle(QString("Total Signal Sum (%1 GV to %2 GV)").arg(m_lowerMomentum).arg(m_upperMomentum));

  TH1D* histogram = new TH1D(qPrintable(title()), qPrintable(title() + ";dE/dx ;entries"), 300, 0, 1000);
  addHistogram(histogram);
}

TotalEnergyDepositionPlot::~TotalEnergyDepositionPlot()
{
}

void TotalEnergyDepositionPlot::processEvent(const QVector<Hit*>& hits, Track* track, SimpleEvent* event)
{

  /*
  //check if everything worked and a track has been fit
  if (!track || !track->fitGood())
    return;

  if (track->chi2() / track->ndf() > 3)
    return;

  //check if track was inside of magnet
  if (!(track->information()->flags() & TrackInformation::InsideMagnet))
    return;



  //get the reconstructed momentum
  double p = track->p(); //GeV

  if( p < m_lowerMomentum || p > m_upperMomentum)
    return;
    */


  if (track->beta() < 1.5 || track->beta() < 0)
    return;

  double signalSum = 0;
  double distanceSum = 0;

  int trdCluster = 0;

  foreach(Hit* clusterHit, event->hits()){
    Cluster* cluster = static_cast<Cluster*>(clusterHit);
    if (cluster->type() == Hit::trd){
      signalSum += cluster->signalHeight();
      trdCluster++;
    }
  }


  if (trdCluster >= 8)
    histogram(0)->Fill(signalSum);

}
