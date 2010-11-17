#ifndef HitsPlot_hh
#define HitsPlot_hh

#include <QVector>

#include "PerdaixDisplay.hh"

class Hit;
class Track;
class TLine;
class TMarker;
class TLatex;

class HitsPlot :
  public PerdaixDisplay
{
  
public:
  HitsPlot();
  ~HitsPlot();
  
public:
  void drawEvent(TCanvas* canvas, const QVector<Hit*>& hits, Track* track = 0);

private:
  void clearHits();

private:
  QVector<TBox*>      m_hits;
  QVector<TLine*>     m_lines;
  QVector<TMarker*>   m_markers;
  TLatex*             m_fitInfo;

};

#endif /* HitsPlot_hh */
