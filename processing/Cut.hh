#ifndef Cut_hh
#define Cut_hh

#include <QVector>

#include "Hit.hh"

class Track;
class Particle;
class SimpleEvent;

class Cut
{

public:
  enum Type {
    rigidity,
    beta,
    tofTimeOverThreshold,
    trdDeposition
  };
  
  Cut(Type type);
  ~Cut();
  void setMin(double min);
  void setMax(double max);
  bool passes(const QVector<Hit*>& clusters, Particle* particle, SimpleEvent* event);

private:
  Type m_type;
  bool m_minIsSet;
  double m_min;
  bool m_maxIsSet;
  double m_max;
  bool passesCuts(double value);
  double sumOfSignalHeights(const Hit::ModuleType type, const Track* track, const QVector<Hit*>& clusters);
  
};

#endif /* Cut_hh */
