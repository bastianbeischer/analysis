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
  bool passes(const QVector<Hit*>&, Particle*, SimpleEvent*);
  void setCuts(CutFilter);

private:
  QList<Cut> m_cuts;

};

#endif
