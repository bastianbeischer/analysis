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
  void setAxisTitle(const QString&, const QString&);
  TGraph* graph(int i = 0);
protected:
  TMultiGraph* m_multiGraph;
private:
  QString m_xAxisTitle;
  QString m_yAxisTitle;
  static const QVector<DrawOption> s_drawOptions;
};

#endif // GRAPHPLOT_HH
