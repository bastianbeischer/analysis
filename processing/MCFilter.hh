#ifndef MCFILTER_HH
#define MCFILTER_HH

#include "Particle.hh"
#include <QFlags>

class SimpleEvent;

class MCFilter
{
public:
  Q_DECLARE_FLAGS(Types, Particle::Type);
public:
  MCFilter(Types = ~Types(0));
  void setTypes(Types types) {m_types = types;}
  Types types() const {return m_types;}
  bool passes(SimpleEvent* event) const;
private:
  Types m_types;
};

#endif // MCFILTER_HH
