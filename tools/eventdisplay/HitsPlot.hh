#ifndef HitsPlot_hh
#define HitsPlot_hh

#include <QVector>

class Hit;
class TBox;
class TCanvas;
class TGaxis;
class TH2D;
class Track;
class TLine;
class TMarker;
class TLatex;

class HitsPlot
{
  
public:
  HitsPlot();
  ~HitsPlot();
  
public:
  double yStretchFactor();
  void draw(TCanvas* canvas, const QVector<Hit*>& hits, Track* track = 0);

private:
  void clear();

private:
  TH2D*               m_positionHist;
  TGaxis*             m_yAxis;

  QVector<TBox*>      m_boxes;
  QVector<TBox*>      m_hits;
  QVector<TLine*>     m_lines;
  QVector<TMarker*>   m_markers;
  TLatex*             m_fitInfo;

  double m_stretchFactor;
};

#endif /* HitsPlot_hh */
