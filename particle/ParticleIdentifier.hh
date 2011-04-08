#ifndef ParticleIdentifier_hh
#define ParticleIdentifier_hh

#include <QList>
#include "Particle.hh"

class ParticleIdentifier
{
  
public:
  ParticleIdentifier();
  ~ParticleIdentifier();
  
  void identify(Particle* particle);
  
private:
  QList<Particle::Type> m_candidates;

};

#endif /* ParticleIdentifier_hh */
