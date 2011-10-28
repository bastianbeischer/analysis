#ifndef ParticleIdentifier_hh
#define ParticleIdentifier_hh

#include "ParticleProperties.hh"

#include <QList>

class Particle;

class ParticleIdentifier
{
  
public:
  ParticleIdentifier();
  ~ParticleIdentifier();

  const QList<const ParticleProperties*>& candidates() const {return m_candidates;}
  void identify(Particle* particle);
  
private:
  QList<const ParticleProperties*> m_candidates;

};

#endif /* ParticleIdentifier_hh */
