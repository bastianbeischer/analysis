#ifndef PostAnalysisCanvas_hh
#define PostAnalysisCanvas_hh

#include "RootStyle.hh"

#include <QVector>
#include <QString>
#include <QRegExp>
#include <QStringList>

class TH1D;
class TH2D;
class TH3D;
class TF1;
class TFile;
class TCanvas;

class PostAnalysisCanvas {
public:
  PostAnalysisCanvas(TCanvas*);
  PostAnalysisCanvas(TFile*, const QString& name);
  virtual ~PostAnalysisCanvas();
  static QStringList savedCanvases(TFile*, const QRegExp& = QRegExp());
  static PostAnalysisCanvas* fromFile(TFile*, const QString& name);
  virtual void draw(TCanvas*);
  void setPalette(RootStyle::PaletteType);
  QString name();
  QVector<TH1D*> histograms1D();
  QVector<TH2D*> histograms2D();
  QVector<TH3D*> histograms3D();
  QVector<TF1*> functions();
  TCanvas* canvas();
private:
  static TCanvas* loadCanvas(TFile*, const QString& name);
  void loadCanvas();
  PostAnalysisCanvas();
  RootStyle::PaletteType m_palette;
  TFile* m_file;
  QString m_name;
  TCanvas* m_canvas;
};

#endif
