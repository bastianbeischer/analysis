#ifndef CutFilter_hh
#define CutFilter_hh

#include "Hit.hh"
#include "Cut.hh"

#include <QList>
#include <QVector>

class Track;
class Particle;
class SimpleEvent;

class CutFilter
{

public:  
  CutFilter();
  ~CutFilter();
  void addCut(Cut);
  void setCuts(const CutFilter&);
  bool passes(const SimpleEvent*) const;
  bool passes(const QVector<Hit*>&, const Particle*) const;

private:
  QList<Cut> m_cuts;

};

#endif
