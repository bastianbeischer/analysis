#ifndef PlotHits_hh
#define PlotHits_hh

#include <QVector>

class Hit;
class TBox;
class TCanvas;
class TGaxis;
class TH2D;
class Track;
class TLine;
class TLatex;

class PlotHits
{
  
public:
  PlotHits();
  ~PlotHits();
  
public:
  void plot(QVector<Hit*> hits, Track* track = 0);

private:
  void clear();
  void saveCanvas(const char* format);

private:
  static unsigned int saves;

  TCanvas*            m_canvas;
  TH2D*               m_positionHist;
  TGaxis*             m_yaxis;

  QVector<TBox*>      m_boxes;
  QVector<TBox*>      m_hits;
  QVector<TLine*>     m_lines;
  TLatex*             m_fitInfo;

};

#endif /* PlotHits_hh */
