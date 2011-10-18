#ifndef GRAPHPLOT_HH
#define GRAPHPLOT_HH

#include "RootPlot.hh"

#include <QString>
#include <QVector>

class TCanvas;
class TGraph;
class TAxis;

class GraphPlot : virtual public RootPlot {
public:
  GraphPlot();
  virtual ~GraphPlot();
  static const QVector<DrawOption>& drawOptions();
  virtual void draw(TCanvas*);
  virtual void unzoom();
  virtual void clear();
  virtual void clear(int);
  int numberOfGraphs() const;
  TGraph* graph(int i = 0);
  void setAxisTitle(const QString&, const QString&);
  void addGraph(TGraph*, DrawOption = P);
  void removeGraph(int i = 0);
  TAxis* xAxis();
  TAxis* yAxis();
private:
  QVector<TGraph*> m_graphs;
  TAxis* m_xAxis;
  TAxis* m_yAxis;
  QString m_xAxisTitle;
  QString m_yAxisTitle;
  QVector<DrawOption> m_drawOptions;
  static const QVector<DrawOption> s_drawOptions;
};

#endif
