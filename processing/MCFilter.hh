#ifndef MCFILTER_HH
#define MCFILTER_HH

#include "Particle.hh"
#include <QFlags>

class SimpleEvent;

class MCFilter
{
public:
  MCFilter(Enums::Particles = Enums::NoParticle);
  void setTypes(Enums::Particles types) {m_types = types;}
  Enums::Particles types() const {return m_types;}
  bool passes(SimpleEvent* event) const;
private:
  Enums::Particles m_types;
};

#endif // MCFILTER_HH
