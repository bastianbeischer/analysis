#ifndef H2DPlot_hh
#define H2DPlot_hh

#include "RootPlot.hh"
#include "RootStyle.hh"

#include <QVector>
#include <QString>

class TCanvas;
class TH2D;

class H2DPlot : virtual public RootPlot {
public:
  H2DPlot();
  virtual ~H2DPlot();
  static const QVector<DrawOption>& drawOptions();
  virtual void draw(TCanvas*);
  virtual void unzoom();
  virtual void clear();
  TH2D* histogram();
  void setHistogram(TH2D*);
  void setPalette(RootStyle::PaletteType);
private:
  TH2D* m_histogram;
  RootStyle::PaletteType m_palette;
  static const QVector<DrawOption> s_drawOptions;
};

#endif
