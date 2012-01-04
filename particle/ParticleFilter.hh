#ifndef ParticleFilter_hh
#define ParticleFilter_hh

#include "Particle.hh"

#include <QFlags>

class ParticleFilter {
public:
  ParticleFilter(Enums::Particles = Enums::NoParticle);
  ~ParticleFilter();

  void setTypes(Enums::Particles types) {m_types = types;}

  Enums::Particles types() const {return m_types;}
  bool passes(Particle*) const;

private:
  Enums::Particles m_types;

};

#endif /* ParticleFilter_hh */
