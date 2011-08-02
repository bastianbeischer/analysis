#ifndef HitsPlot_hh
#define HitsPlot_hh

#include <QVector>

#include "PerdaixDisplay.hh"
#include "EventDestination.hh"

class Hit;
class Particle;
class TEllipse;
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
  void processEvent(const QVector<Hit*>&, const Particle* const = 0, const SimpleEvent* const = 0);
  void setDrawAllClusters(bool);
private:
  void drawHits();
  void clearHits();

private:
  QVector<TBox*>      m_hits;
  QVector<TEllipse*>  m_hitsTRD;
  QVector<TLine*>     m_lines;
  QVector<TMarker*>   m_markers;
  TLatex*             m_fitInfo;
  QList<TGraph*>      m_trajectoriesXZ;
  QList<TGraph*>      m_trajectoriesYZ;
  bool m_drawAllClusters;
};

#endif /* HitsPlot_hh */
