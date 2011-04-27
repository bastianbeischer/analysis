#ifndef ParticleFilter_hh
#define ParticleFilter_hh

#include "Particle.hh"

#include <QFlags>

class ParticleFilter
{
  
public:
  Q_DECLARE_FLAGS(Types, Particle::Type);

public:
  ParticleFilter(Types = ~Types(0));
  ~ParticleFilter();
  
  void setTypes(Types types) {m_types = types;}

  Types types() const {return m_types;}
  bool passes(Particle*) const;

private:
  Types m_types;
  
};

Q_DECLARE_OPERATORS_FOR_FLAGS(ParticleFilter::Types);

#endif /* ParticleFilter_hh */
