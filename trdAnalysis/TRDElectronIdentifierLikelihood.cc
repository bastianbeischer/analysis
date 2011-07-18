#include "TRDElectronIdentifierLikelihood.hh"

#include <QVector>
#include <math.h>

#include "TRDReconstruction.hh"
#include "TRDLikelihoods.hh"
#include "SimpleEvent.hh"
#include "Hit.hh"
#include "Cluster.hh"
#include "Particle.hh"

#include <TF1.h>

TRDElectronIdentifierLikelihood::TRDElectronIdentifierLikelihood()
  : m_logLHCut(0.5)
{
}

bool TRDElectronIdentifierLikelihood::isElectronish(const QVector<Hit*>& hits, const Particle* particle, const SimpleEvent* event, bool &ok)
{
  double logLH = 0;
  return isElectronish(hits, particle, event, ok, logLH);
}

bool TRDElectronIdentifierLikelihood::isElectronish(const QVector<Hit*>& hits, const Particle* particle, const SimpleEvent* event, bool &ok, double &logLH)
{
  //use general trd cuts here:
  if (TRDReconstruction::globalTRDCuts(hits, particle, event)) {
    ok = false;
    return false;
  }

  //now get all relevant energy deposition for this specific plot and all length
  QList<double> values;
  QList<int> layers;

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
      if (TRDReconstruction::calculateLengthInTube)
          distanceInTube = TRDReconstruction::distanceOnTrackThroughTRDTube(subHit, track);
      if (distanceInTube > 0) {
        values << hit->signalHeight() / distanceInTube;
        layers << TRDReconstruction::TRDLayerNo(subHit->detId());
      }
    } // subhits in cluster
  } // all hits

  //check again if the trdhits are still on the fitted track and fullfill the minTRDLayerCut
  int hitsWhichAreOnTrack = values.size();
  if (hitsWhichAreOnTrack < TRDReconstruction::minLayerCut) {
    ok = false;
    return false;
  }

  //can be reconstructed
  ok = true;

  //calculate likelihoods:
  TRDLikelihoods* lhs = TRDLikelihoods::instance();
  QList<double> lhsNonTR;
  QList<double> lhsTR;
  for (int i = 0; i < values.size(); ++i) {
    lhsNonTR << lhs->getLHFunctionNonTR()->Eval(values.at(i));
    lhsTR << lhs->getLHFunctionTRLayer(layers.at(i))->Eval(values.at(i));
  }

  //calculate combined likelihoods
  double nonTRLH = 1.;
  foreach(double layerLH, lhsNonTR)
    nonTRLH *= layerLH;
  nonTRLH = pow(nonTRLH, 1.0 / lhsNonTR.size() );

  double trLH = 1;
  foreach(double layerLH, lhsTR)
    trLH *= layerLH;
  trLH = pow(trLH, 1.0 / lhsTR.size() );

  //log likelihood:
  logLH = -log(trLH / ( trLH + nonTRLH)) ;

  return logLH < m_logLHCut;
}
