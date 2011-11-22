#include "SpectrometerReconstruction.hh"
#include "Likelihood.hh"
#include "KineticVariable.hh"
#include "AnalyzedEvent.hh"
#include "Particle.hh"

SpectrometerReconstruction::SpectrometerReconstruction(Enums::LikelihoodVariables variables, Enums::Particles particles, bool additionalInformation)
  : Reconstruction(variables, particles)
{
  m_method = additionalInformation ? Enums::SpectrometerExternalInformation : Enums::Spectrometer;
}

void SpectrometerReconstruction::identify(AnalyzedEvent* event)
{
  m_event = event;
  m_indexOfGlobalMinimum = 0;

  QVector<Enums::Particle>::ConstIterator particleIt = m_particles.begin();
  QVector<Enums::Particle>::ConstIterator particleEnd = m_particles.end();
  QVector<QPointF>::Iterator pointIt = m_minima.begin();
  for (int it = 0; particleIt != particleEnd; ++it, ++particleIt, ++pointIt) {
    Q_ASSERT(*particleIt != Enums::NoParticle);
    double curvature = 1. / event->particle()->track()->rigidity();
    Hypothesis* h = new Hypothesis(*particleIt, curvature);
    h->setProbability(1); //TODO
    event->particle()->addHypothesis(m_method, h);
  }
}
