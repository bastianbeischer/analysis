#ifndef ParticleIdentifier_hh
#define ParticleIdentifier_hh

#include <QList>
#include "ParticleProperties.hh"

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
