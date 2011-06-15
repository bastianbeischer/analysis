#ifndef MCFILTER_HH
#define MCFILTER_HH

#include <QVector>
#include <QList>

#include "Particle.hh"
#include <QFlags>

class SimpleEvent;
class Hit;

class MCFilter
{
public:
  Q_DECLARE_FLAGS(Types, Particle::Type);
public:
  MCFilter(Types = ~Types(0));
  void setTypes(Types types) {m_types = types;}
  Types types() const {return m_types;}
  bool passes(const QVector<Hit*>&, Particle*, SimpleEvent* event) const;
private:
  Types m_types;
};

#endif // MCFILTER_HH
