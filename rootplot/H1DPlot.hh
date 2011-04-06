#ifndef H1DPlot_hh
#define H1DPlot_hh

#include "RootPlot.hh"

#include <QVector>

class TCanvas;
class TH1D;
class THStack;
class TAxis;

class H1DPlot : virtual public RootPlot {
public:
  H1DPlot();
  virtual ~H1DPlot();
  static const QVector<DrawOption>& drawOptions();
  virtual void draw(TCanvas*);
  virtual void unzoom();
  virtual void clear();
  virtual void clear(int);
  int numberOfHistograms();
  TH1D* histogram(int i = 0);
  void setAxisTitle(const QString&, const QString&);
  void addHistogram(TH1D*, DrawOption = BLANK);
protected:
  TAxis* m_xAxis;
  TAxis* m_yAxis;
private:
  THStack* m_stack;
  QString m_xAxisTitle;
  QString m_yAxisTitle;
  static const QVector<DrawOption> s_drawOptions;
};

#endif
