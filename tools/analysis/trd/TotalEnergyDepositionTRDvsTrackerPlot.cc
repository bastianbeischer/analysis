#include "TotalEnergyDepositionTRDvsTrackerPlot.hh"

#include <TPad.h>
#include <TH2D.h>

#include "Particle.hh"
#include "Track.hh"
#include "ParticleInformation.hh"
#include "SimpleEvent.hh"
#include "Cluster.hh"
#include "Hit.hh"

#include "TRDCalculations.hh"

TotalEnergyDepositionTRDvsTrackerPlot::TotalEnergyDepositionTRDvsTrackerPlot() :
    AnalysisPlot(AnalysisPlot::MiscellaneousTRD),
  H2DPlot()
{

  setTitle("Mean dE_dx TRD vs. tracker");

  TH2D* histogram = new TH2D(qPrintable(title()), qPrintable(title() + ";tracker mean amplitude tracker;TRD mean dE/dx"), 50, 0, 8000, 50, 0, 30);
  addHistogram(histogram);
}

TotalEnergyDepositionTRDvsTrackerPlot::~TotalEnergyDepositionTRDvsTrackerPlot()
{
}

void TotalEnergyDepositionTRDvsTrackerPlot::processEvent(const QVector<Hit*>& hits, Particle* particle, SimpleEvent*)
{
  const Track* track = particle->track();

  //check if everything worked and a track has been fit
  if (!track || !track->fitGood())
    return;

  if (particle->information()->numberOfTrackerLayers() < 7)
    return;

  if (track->chi2() / track->ndf() > 5)
    return;

  //check if track was inside of magnet
  if (!(particle->information()->flags() & ParticleInformation::InsideMagnet))
    return;


  /*
  //get the reconstructed momentum
  double p = track->p(); //GeV

  if( p < m_lowerMomentum || p > m_upperMomentum)
    return;

  if (particle->beta() < 1.5 || particle->beta() < 0)
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
      double max = 0.;
      for(unsigned short i = 0; i < cluster->hits().size(); ++i) {
        double amplitude = cluster->hits().at(i)->signalHeight();
        if (amplitude > max) {
          max = amplitude;
        }
      }
      trackerCluster++;
      //cluster amplitude - max channel
      signalSumTracker += cluster->signalHeight() - max;
    }
  }

  //cut on trd cluster
  if (trdCluster < 7)
    return;

  histogram()->Fill(signalSumTracker / trackerCluster, signalSumTRD / distanceSumTRD) ;


}
