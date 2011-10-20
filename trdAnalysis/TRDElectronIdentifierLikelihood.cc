#include "TRDElectronIdentifierLikelihood.hh"

#include <QVector>
#include <math.h>

#include "TRDReconstruction.hh"
#include "TRDLikelihoods.hh"
#include "SimpleEvent.hh"
#include "Hit.hh"
#include "Cluster.hh"
#include "Particle.hh"
#include "AnalyzedEvent.hh"

#include <TF1.h>

TRDElectronIdentifierLikelihood::TRDElectronIdentifierLikelihood()
  : m_logLHCut(0.5)
{
}

bool TRDElectronIdentifierLikelihood::isElectronish(const AnalyzedEvent* event, bool &ok)
{
  double logLH = 0;
  return isElectronish(event, ok, logLH);
}

bool TRDElectronIdentifierLikelihood::isElectronish(const AnalyzedEvent* event, bool &ok, double &logLH)
{
  const TRDReconstruction* trdReconst = event->particle()->trdReconstruction();
  if (!(trdReconst->flags() & TRDReconstruction::GoodTRDEvent)) {
    ok = false;
    return false;
  }

  //can be reconstructed
  ok = true;

  QList<double> values;
  if (TRDReconstruction::s_calculateLengthInTube) {
    for (int i = 0; i < 8; ++i)
      values << trdReconst->energyDepositionForLayer(i).edepOnTrackPerLength;
  } else {
    for (int i = 0; i < 8; ++i)
      values << trdReconst->energyDepositionForLayer(i).edepOnTrack;
  }

  //calculate likelihoods:
  TRDLikelihoods* lhs = TRDLikelihoods::instance();
  QList<double> lhsNonTR;
  QList<double> lhsTR;

  //get lhs for all 8 layers
  for (int i = 0; i < 8; ++i) {
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
