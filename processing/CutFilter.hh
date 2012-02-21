#ifndef CutFilter_hh
#define CutFilter_hh

#include "Cut.hh"

#include <QList>
#include <QVector>
#include <QString>

class Track;
class Particle;
class SimpleEvent;
class AnalyzedEvent;

class CutFilter
{
public:
  CutFilter();
  CutFilter(const QString&);
  ~CutFilter();
  void clear();
  void addCut(Cut);
  void setCuts(const CutFilter&);
  const QList<Cut>& cuts() const;
  bool passes(const SimpleEvent*) const;
  bool passes(const AnalyzedEvent*) const;
  QString toString() const;
private:
  QList<Cut> m_cuts;
};

#endif
