#ifndef H2DPlot_hh
#define H2DPlot_hh

#include "RootPlot.hh"
#include "RootStyle.hh"

#include <QVector>
#include <QString>

class TCanvas;
class TH2D;
class THStack;
class TAxis;

class H2DPlot : virtual public RootPlot {
public:
  H2DPlot();
  virtual ~H2DPlot();
  static const QVector<DrawOption>& drawOptions();
  virtual void draw(TCanvas*);
  virtual void unzoom();
  virtual void clear(int);
  virtual void clear();
  int numberOfHistograms();
  TH2D* histogram(int i = 0);
  void setAxisTitle(const QString&, const QString&, const QString&);
  void addHistogram(TH2D*, DrawOption = COLZ);
  void setPalette(RootStyle::PaletteType);
  TAxis* xAxis();
  TAxis* yAxis();
  TAxis* zAxis();
private:
  TAxis* m_xAxis;
  TAxis* m_yAxis;
  TAxis* m_zAxis;
  QVector<TH2D*> m_histograms;
  RootStyle::PaletteType m_palette;
  QString m_xAxisTitle;
  QString m_yAxisTitle;
  QString m_zAxisTitle;
  QVector<DrawOption> m_drawOptions;
  static const QVector<DrawOption> s_drawOptions;
};

#endif
