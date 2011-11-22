#include "WeightedMeanReconstruction.hh"
#include "Likelihood.hh"
#include "KineticVariable.hh"
#include "AnalyzedEvent.hh"
#include "Particle.hh"
#include "Helpers.hh"

#include <cmath>

WeightedMeanReconstruction::WeightedMeanReconstruction(Enums::LikelihoodVariables variables, Enums::Particles particles, bool additionalInformation)
  : Reconstruction(variables, particles)
{
  m_method = additionalInformation ? Enums::WeightedMeanExternalInformation : Enums::WeightedMean;
}

WeightedMeanReconstruction::~WeightedMeanReconstruction()
{

}

void WeightedMeanReconstruction::identify(AnalyzedEvent* event)
{
  m_event = event;
  m_indexOfGlobalMinimum = 0;

  QVector<Enums::Particle>::ConstIterator particleIt = m_particles.begin();
  QVector<Enums::Particle>::ConstIterator particleEnd = m_particles.end();
  QVector<QPointF>::Iterator pointIt = m_minima.begin();
  for (int it = 0; particleIt != particleEnd; ++it, ++particleIt, ++pointIt) {
    Q_ASSERT(*particleIt != Enums::NoParticle);
    double enumeratur = 0;
    double denumerator = 0;
    double rigidity = event->particle()->track()->rigidity();
    if (!qIsNull(rigidity)) {
      double curvature = 1. / event->particle()->track()->rigidity();
      double relativeSigmaCurvature = Helpers::trackerResolution(curvature, *particleIt);
      double sigmaCurvature = curvature * relativeSigmaCurvature;
      double sigmaCurvature2 = sigmaCurvature * sigmaCurvature;
      enumeratur+= curvature/sigmaCurvature2;
      denumerator+= 1./sigmaCurvature2;
    }
    double inverseBeta = 1. / event->particle()->beta();
    if (inverseBeta > 1) {
      double tofResolution = Helpers::tofResolution();
      double mass = ParticleProperties(*particleIt).mass();
      double curvatureTof = sqrt(inverseBeta*inverseBeta - 1) / mass;
      double sigmaCurvatureTof = tofResolution * inverseBeta / sqrt(inverseBeta*inverseBeta - 1);
      double sigmaCurvatureTof2 = sigmaCurvatureTof * sigmaCurvatureTof;
      enumeratur+= curvatureTof/sigmaCurvatureTof2;
      denumerator+= 1./sigmaCurvatureTof2;
    }
    Hypothesis* h = new Hypothesis(*particleIt, qIsNull(denumerator) ? 0 : enumeratur / denumerator);
    h->setProbability(1); //TODO
    event->particle()->addHypothesis(m_method, h);
  }
}
