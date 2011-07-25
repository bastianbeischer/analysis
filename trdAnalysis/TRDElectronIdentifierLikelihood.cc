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

bool TRDElectronIdentifierLikelihood::isElectronish(const QVector<Hit*>&, const Particle* particle, const SimpleEvent*, bool &ok, double &logLH)
{
  const TRDReconstruction* trdReconst = particle->trdReconstruction();
  if (!(trdReconst->flags() & TRDReconstruction::GoodTRDEvent)) {
    ok = false;
    return false;
  }

  //can be reconstructed
  ok = true;

  const QVector<double>& values = TRDReconstruction::s_calculateLengthInTube ?
        trdReconst->energyDepositionOnTrackPerLengthForLayers() : trdReconst->energyDepositionOnTrackForLayers();

  //calculate likelihoods:
  TRDLikelihoods* lhs = TRDLikelihoods::instance();
  QList<double> lhsNonTR;
  QList<double> lhsTR;

  for (int i = 0; i < values.size(); ++i) {
    if (values.at(i) == 0.)
      continue;
    lhsNonTR << lhs->lhFunctionNonTR()->Eval(values.at(i));
    lhsTR << lhs->lhFunctionTRLayer(i)->Eval(values.at(i));
  }

  //calculate combined likelihoods
  double nonTRLH = 1.;
  foreach(double layerLH, lhsNonTR)
    nonTRLH *= layerLH;
  nonTRLH = pow(nonTRLH, 1. / lhsNonTR.size() );

  double trLH = 1.;
  foreach(double layerLH, lhsTR)
    trLH *= layerLH;
  trLH = pow(trLH, 1. / lhsTR.size() );

  //log likelihood:
  logLH = -log(trLH / ( trLH + nonTRLH)) ;

  return logLH < m_logLHCut;
}
