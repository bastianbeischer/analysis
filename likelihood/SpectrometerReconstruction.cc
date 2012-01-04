#include "SpectrometerReconstruction.hh"
#include "Likelihood.hh"
#include "KineticVariable.hh"
#include "AnalyzedEvent.hh"
#include "Particle.hh"

#include <cfloat>

SpectrometerReconstruction::SpectrometerReconstruction(Enums::LikelihoodVariables variables, Enums::Particles particles, bool additionalInformation)
  : Reconstruction(variables, particles)
{
  m_externalInformation = additionalInformation;
  m_method = additionalInformation ? Enums::SpectrometerExternalInformation : Enums::Spectrometer;
  m_minima = QVector<QPointF>(m_particles.count());
}

void SpectrometerReconstruction::identify(AnalyzedEvent* event)
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
    double curvature = 1. / event->particle()->track()->rigidity();
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
