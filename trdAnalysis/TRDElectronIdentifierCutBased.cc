#include "TRDElectronIdentifierCutBased.hh"

#include <QVector>

#include "TRDCalculations.hh"
#include "SimpleEvent.hh"
#include "Hit.hh"
#include "Cluster.hh"
#include "Particle.hh"


unsigned int m_layerCut;
double m_energyDepositionCut;
double m_energyDepositionPerLengthCut;

TRDElectronIdentifierCutBased::TRDElectronIdentifierCutBased()
  : m_layerCut(5)
  , m_energyDepositionCut(5.) // keV
  , m_energyDepositionPerLengthCut(5./4.) // keV/mm
{
}

bool TRDElectronIdentifierCutBased::isElectronish(const QVector<Hit*>& hits, const Particle* particle, const SimpleEvent* event, bool &ok)
{
  //use general trd cuts here:
  if (TRDCalculations::globalTRDCuts(hits, particle, event)) {
    ok = false;
    return false;
  }

  //now get all relevant energy deposition for this specific plot and all length
  QList<double> values;

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
      if (distanceInTube > 0)
        values << hit->signalHeight() / distanceInTube;
    } // subhits in cluster
  } // all hits

  //check again if the trdhits are still on the fitted track and fullfill the minTRDLayerCut
  unsigned int hitsWhichAreOnTrack = values.size();
  if (hitsWhichAreOnTrack < TRDCalculations::minTRDLayerCut) {
    ok = false;
    return false;
  }

  //can be reconstructed
  ok = true;

  qSort(values) ;
  if (values.at(m_layerCut-1) > m_energyDepositionPerLengthCut) {
    return false;
  } else {
    return true;
  }

}
