#ifndef H3DPlot_hh
#define H3DPlot_hh

#include "RootPlot.hh"
#include "RootStyle.hh"

#include <QVector>
#include <QString>

class TCanvas;
class TH3D;
class TAxis;

class H3DPlot : virtual public RootPlot {
public:
  H3DPlot();
  virtual ~H3DPlot();
  static const QVector<DrawOption>& drawOptions();
  virtual void draw(TCanvas*);
  virtual void unzoom();
  virtual void clear(int);
  virtual void clear();
  int numberOfHistograms() const;
  TH3D* histogram(int i = 0) const;
  void setAxisTitle(const QString&, const QString&, const QString&);
  void addHistogram(TH3D*, DrawOption = COLZ);
  void setPalette(RootStyle::PaletteType);
  TAxis* xAxis() const;
  TAxis* yAxis() const;
  TAxis* zAxis() const;
private:
  TAxis* m_xAxis;
  TAxis* m_yAxis;
  TAxis* m_zAxis;
  QVector<TH3D*> m_histograms;
  RootStyle::PaletteType m_palette;
  QString m_xAxisTitle;
  QString m_yAxisTitle;
  QString m_zAxisTitle;
  QVector<DrawOption> m_drawOptions;
  static const QVector<DrawOption> s_drawOptions;
};

#endif
