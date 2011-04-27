#ifndef PostAnalysisCanvas_hh
#define PostAnalysisCanvas_hh

#include "RootStyle.hh"

#include <QVector>
#include <QString>

class TH2D;
class TH1D;
class TF1;
class TFile;
class TCanvas;

class PostAnalysisCanvas {
public:
  PostAnalysisCanvas(TFile*, const QString& name);
  virtual ~PostAnalysisCanvas();
  void draw(TCanvas*);
  void setPalette(RootStyle::PaletteType);
  QString name();
  QVector<TH2D*> histograms2D();
  QVector<TH1D*> histograms1D();
  QVector<TF1*> functions();
private:
  RootStyle::PaletteType m_palette;
  TCanvas* m_canvas;
};

#endif
