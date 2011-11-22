#include "TOFReconstruction.hh"
#include "Likelihood.hh"
#include "KineticVariable.hh"
#include "AnalyzedEvent.hh"
#include "Particle.hh"

TOFReconstruction::TOFReconstruction(Enums::LikelihoodVariables variables, Enums::Particles particles, bool additionalInformation)
  : Reconstruction(variables, particles)
{
  m_method = additionalInformation ? Enums::TOFExternalInformation : Enums::TOF; 
}

TOFReconstruction::~TOFReconstruction()
{

}

void TOFReconstruction::identify(AnalyzedEvent* event)
{
  m_event = event;
  m_indexOfGlobalMinimum = 0;

  QVector<Enums::Particle>::ConstIterator particleIt = m_particles.begin();
  QVector<Enums::Particle>::ConstIterator particleEnd = m_particles.end();
  QVector<QPointF>::Iterator pointIt = m_minima.begin();
  for (int it = 0; particleIt != particleEnd; ++it, ++particleIt, ++pointIt) {
    Q_ASSERT(*particleIt != Enums::NoParticle);
    double beta = event->particle()->beta();
    double beta2 = beta * beta;
    double mass = ParticleProperties(*particleIt).mass();
    double mass2 = mass * mass;
    double curvature = (1. - beta2) / (mass2 * beta2);
    Hypothesis* h = new Hypothesis(*particleIt, curvature);
    h->setProbability(1); //TODO
    event->particle()->addHypothesis(m_method, h);
  }
}
