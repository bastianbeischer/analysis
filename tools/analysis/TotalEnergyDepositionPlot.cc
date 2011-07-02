#include "TotalEnergyDepositionPlot.hh"

#include <TH1D.h>

#include "Particle.hh"
#include "Track.hh"
#include "ParticleInformation.hh"
#include "SimpleEvent.hh"
#include "Cluster.hh"
#include "Hit.hh"

#include "TRDCalculations.hh"

TotalEnergyDepositionPlot::TotalEnergyDepositionPlot()
  : AnalysisPlot(AnalysisPlot::MiscellaneousTRD)
  , H1DPlot()
{

  setTitle("Total Signal Sum");

  TH1D* histogram = new TH1D(qPrintable(title()), qPrintable(title() + ";total signal sum;entries"), 100, 0, 40);
  addHistogram(histogram);
}

TotalEnergyDepositionPlot::~TotalEnergyDepositionPlot()
{
}

void TotalEnergyDepositionPlot::processEvent(const QVector<Hit*>& hits, Particle* particle, SimpleEvent* event)
{
  if (!TRDCalculations::globalTRDCuts(hits, particle, event))
      return;

  //now get all relevant energy deposition for this specific plot and all length
  QList<double> lengthList;
  QList<double> signalList;

  const Track* track = particle->track();
  for (QVector<Hit*>::const_iterator it = track->hits().begin(); it != track->hits().end(); ++it) {
    Hit* hit = *it;
    if (hit->type() != Hit::trd)
      continue;
    Cluster* cluster = static_cast<Cluster*>(hit);
    std::vector<Hit*>& subHits = cluster->hits();
    const std::vector<Hit*>::const_iterator subHitsEndIt = subHits.end();
    for (std::vector<Hit*>::const_iterator it = subHits.begin(); it != subHitsEndIt; ++it) {
      Hit* subHit = *it;
      double distanceInTube = 1.; //default length in trd tube, if no real calcultaion is performed
      if (TRDCalculations::calculateLengthInTube)
          distanceInTube = TRDCalculations::distanceOnTrackThroughTRDTube(subHit, track);
      if (distanceInTube > 0) {
        signalList << subHit->signalHeight();
        lengthList << distanceInTube;
      }
    } // subhits in cluster
  } // all hits

  /* now fill the mean of all gathered data
      - one value for a single tube
      - normally also one value for a module (except no length is calculated and 2 tubes show a signal)
      - several signals for the complete trd
  */

  //check again if the trdhits are still on the fitted track and fullfill the minTRDLayerCut
  unsigned int hitsWhichAreOnTrack = signalList.size();
  if (hitsWhichAreOnTrack < TRDCalculations::minTRDLayerCut)
    return;

  double signalSum = 0;
  for (int i = 0; i < signalList.size(); ++i)
    signalSum +=  signalList.at(i) / lengthList.at(i);
  histogram()->Fill(signalSum);
}
