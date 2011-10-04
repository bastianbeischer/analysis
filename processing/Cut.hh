#ifndef Cut_hh
#define Cut_hh

#include <QVector>

#include "Hit.hh"
#include "Enums.hh"

class Track;
class Particle;
class SimpleEvent;

class Cut {
public:
  Cut(Enums::Cut);
  ~Cut();
  void setMin(double min);
  void setMax(double max);
  bool passes (const SimpleEvent* event) const;
  bool passes (const QVector<Hit*>& clusters, const Particle* particle) const;
  Enums::Cut type() const {return m_type;}
  bool valid() const {return (m_minIsSet || m_maxIsSet);}

private:
  Enums::Cut m_type;
  bool m_minIsSet;
  double m_min;
  bool m_maxIsSet;
  double m_max;
  bool passesCuts(double value) const;
  double sumOfSignalHeights(const Hit::ModuleType type, const Track* track, const QVector<Hit*>& clusters) const;
  
};

#endif /* Cut_hh */
