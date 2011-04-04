#ifndef GRAPHPLOT_HH
#define GRAPHPLOT_HH

#include "RootPlot.hh"

#include <QString>
#include <QVector>

class TMultiGraph;
class TCanvas;
class TGraph;

class GraphPlot : virtual public RootPlot {
public:
  GraphPlot();
  virtual ~GraphPlot();
  static const QVector<DrawOption>& drawOptions();
  virtual void draw(TCanvas*);
  virtual void unzoom();
  virtual void clear();
  virtual void clear(int);
  void addGraph(TGraph*, const char* = "");
  int numberOfGraphs() const;
  TGraph* graph(int i = 0);
  TMultiGraph* multiGraph();
protected:
  TMultiGraph* m_multiGraph;
  static const QVector<DrawOption> s_drawOptions;
};

#endif // GRAPHPLOT_HH
