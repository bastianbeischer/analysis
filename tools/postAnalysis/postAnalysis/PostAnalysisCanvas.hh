#ifndef PostAnalysisCanvas_hh
#define PostAnalysisCanvas_hh

#include "RootStyle.hh"

#include <QVector>
#include <QString>

class TH1D;
class TH2D;
class TH3D;
class TF1;
class TFile;
class TCanvas;

class PostAnalysisCanvas {
public:
  PostAnalysisCanvas(TCanvas*);
  virtual ~PostAnalysisCanvas();
  static PostAnalysisCanvas* fromFile(TFile*, const QString& name);
  void draw(TCanvas*);
  void setPalette(RootStyle::PaletteType);
  QString name();
  QVector<TH1D*> histograms1D();
  QVector<TH2D*> histograms2D();
  QVector<TH3D*> histograms3D();
  QVector<TF1*> functions();
  TCanvas* canvas() const {return m_canvas;}
private:
  void setCanvas(TCanvas* canvas) {m_canvas = canvas;}
  PostAnalysisCanvas();
  RootStyle::PaletteType m_palette;
  TCanvas* m_canvas;
};

#endif
