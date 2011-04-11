#ifndef ParticleIdentifier_hh
#define ParticleIdentifier_hh

#include <QList>
#include "Particle.hh"

class ParticleIdentifier
{
  
public:
  ParticleIdentifier();
  ~ParticleIdentifier();

  const QList<const Particle*>& candidates() const {return m_candidates;}
  void identify(Particle* particle);
  
private:
  QList<const Particle*> m_allParticles;
  QList<const Particle*> m_candidates;

};

#endif /* ParticleIdentifier_hh */
