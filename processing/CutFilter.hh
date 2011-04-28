#ifndef CutFilter_hh
#define CutFilter_hh

#include "Hit.hh"

#include <QList>
#include <QVector>

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

class CutFilter
{

public:  
  CutFilter();
  ~CutFilter();
  void addCut(Cut cut);
  bool passes(const QVector<Hit*>& clusters, Particle* particle, SimpleEvent* event);
  void setCuts(CutFilter cuts);

private:
  QList<Cut> m_cuts;
};

#endif
