#ifndef HitsPlot_hh
#define HitsPlot_hh

#include <QVector>

#include "PerdaixDisplay.hh"
#include "EventDestination.hh"

class Hit;
class Track;
class TLine;
class TMarker;
class TLatex;
class TGraph;

class HitsPlot :
  public PerdaixDisplay,
  public EventDestination
{
  
public:
  HitsPlot();
  ~HitsPlot();
  
public:
  void processEvent(const QVector<Hit*>& hits, Track* track = 0, SimpleEvent* = 0);

private:
  void drawHits();
  void clearHits();

private:
  QVector<TBox*>      m_hits;
  QVector<TLine*>     m_lines;
  QVector<TMarker*>   m_markers;
  TLatex*             m_fitInfo;
  TGraph*             m_trajectoryXZ;
  TGraph*             m_trajectoryYZ;

};

#endif /* HitsPlot_hh */
