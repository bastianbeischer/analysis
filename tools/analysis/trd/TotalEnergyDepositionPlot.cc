#include "TotalEnergyDepositionPlot.hh"

#include <TH1D.h>

#include "Particle.hh"
#include "Track.hh"
#include "ParticleInformation.hh"
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

void TotalEnergyDepositionPlot::processEvent(const QVector<Hit*>&, const Particle* const particle, const SimpleEvent* const event)
{
  //const Track* track = particle->track();

  /*
  //check if everything worked and a track has been fit
  if (!track || !track->fitGood())
    return;

  if (track->chi2() / track->ndf() > 3)
    return;

  //check if track was inside of magnet
  if (!(particle->information()->flags() & ParticleInformation::InsideMagnet))
    return;



  //get the reconstructed momentum
  double p = track->p(); //GeV

  if( p < m_lowerMomentum || p > m_upperMomentum)
    return;
    */


  if (particle->beta() < 1.5 || particle->beta() < 0)
    return;

  double signalSum = 0;
  // double distanceSum = 0;

  int trdCluster = 0;

  const std::vector<Hit*>& eventHits = event->hits();
  std::vector<Hit*>::const_iterator endIt = eventHits.end();
  for (std::vector<Hit*>::const_iterator it = eventHits.begin(); it != endIt; ++it) {
    Hit* clusterHit = *it;
    Cluster* cluster = static_cast<Cluster*>(clusterHit);
    if (cluster->type() == Hit::trd){
      signalSum += cluster->signalHeight();
      trdCluster++;
    }
  }


  if (trdCluster >= 8)
    histogram(0)->Fill(signalSum);

}
