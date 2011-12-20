#include "TOFReconstruction.hh"
#include "Likelihood.hh"
#include "KineticVariable.hh"
#include "AnalyzedEvent.hh"
#include "Particle.hh"

#include <cfloat>
#include <cmath>

TOFReconstruction::TOFReconstruction(Enums::LikelihoodVariables variables, Enums::Particles particles, bool additionalInformation)
  : Reconstruction(variables, particles)
{
  m_externalInformation = additionalInformation;
  m_method = additionalInformation ? Enums::TOFExternalInformation : Enums::TOF; 
  m_minima = QVector<QPointF>(m_particles.count());
}

TOFReconstruction::~TOFReconstruction()
{
}

void TOFReconstruction::identify(AnalyzedEvent* event)
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
    double beta = event->particle()->beta();
    double curvature = beta < 0 ? -1e-9 : 1e-9;
    if (qAbs(beta) < 1) {
      double beta2 = beta * beta;
      ParticleProperties properties(*particleIt);
      double mass = properties.mass();
      double charge = properties.charge();
      curvature = charge * sqrt(1 - beta2) / (mass * beta);
    }
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
