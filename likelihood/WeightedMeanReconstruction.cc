#include "WeightedMeanReconstruction.hh"
#include "Likelihood.hh"
#include "KineticVariable.hh"
#include "AnalyzedEvent.hh"
#include "Particle.hh"
#include "Helpers.hh"

#include <cmath>
#include <cfloat>

WeightedMeanReconstruction::WeightedMeanReconstruction(Enums::LikelihoodVariables variables, Enums::Particles particles, bool additionalInformation)
  : Reconstruction(variables, particles)
{
  m_externalInformation = additionalInformation;
  m_method = additionalInformation ? Enums::WeightedMeanExternalInformation : Enums::WeightedMean;
  m_minima = QVector<QPointF>(m_particles.count());
}

WeightedMeanReconstruction::~WeightedMeanReconstruction()
{
}

void WeightedMeanReconstruction::identify(AnalyzedEvent* event)
{
  m_event = event;
  bool goodInterpolation = true;
  m_indexOfGlobalMinimum = 0;

  QVector<QPointF>::Iterator pointIt = m_minima.begin();
  QVector<QPointF>::Iterator pointEnd = m_minima.end();
  for (; pointIt != pointEnd; ++pointIt)
    pointIt->setY(DBL_MAX);

  pointIt = m_minima.begin();
  QVector<Enums::Particle>::ConstIterator particleIt = m_particles.begin();
  QVector<Enums::Particle>::ConstIterator particleEnd = m_particles.end();
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
    double beta = event->particle()->beta();
    if (!qIsNull(beta) && qAbs(beta) < 1) {
      double inverseBeta = 1. / event->particle()->beta();
      double tofResolution = Helpers::tofResolution();
      double mass = ParticleProperties(*particleIt).mass();
      double curvatureTof = sqrt(inverseBeta*inverseBeta - 1) / mass;
      double sigmaCurvatureTof = tofResolution * inverseBeta / sqrt(inverseBeta*inverseBeta - 1);
      double sigmaCurvatureTof2 = sigmaCurvatureTof * sigmaCurvatureTof;
      enumeratur+= curvatureTof/sigmaCurvatureTof2;
      denumerator+= 1./sigmaCurvatureTof2;
    }
    double curvature = qIsNull(denumerator) ? 0 : enumeratur / denumerator;
    Hypothesis* h = new Hypothesis(*particleIt, curvature);
    double value = eval(event, *h, &goodInterpolation);
    if (value < m_minima[m_indexOfGlobalMinimum].y())
      m_indexOfGlobalMinimum = it;
    h->setLogLikelihood(value);
    event->particle()->addHypothesis(m_method, h);
    Q_ASSERT(pointIt != m_minima.end());
    *pointIt = QPointF(curvature, value);
  }
}
