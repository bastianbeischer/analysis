#include "Helpers.hh"

#include <cmath>

#include <QVector>
#include <QString>

#include <TH1.h>
#include <TGraphAsymmErrors.h>

namespace Helpers
{
  double addQuad(double a, double b, double c, double d, double e) {
    return sqrt(a*a + b*b + c*c + d*d + e*e);
  }

  QVector<double> logBinning(unsigned int nBins, double min, double max) {
    QVector<double> binning;
    const double delta = (log(max) / log(min) - 1) / nBins;
    for (unsigned int i = 0; i <= nBins; ++i) {
      binning.append(pow(min, 1 + delta * i));
    }
    return binning;
  }
  
  void updateMirroredHistogram(TH1D* histogramToUpdate, TH1D* dataHistogram)
  {
    const int nBins = dataHistogram->GetNbinsX();
    for (int oldBin = 0; oldBin <= nBins+1; ++oldBin) {
      int newBin = nBins + 1 - oldBin;
      histogramToUpdate->SetBinContent(newBin, dataHistogram->GetBinContent(oldBin));
      histogramToUpdate->SetBinError(newBin, dataHistogram->GetBinError(oldBin));
    }
    histogramToUpdate->SetEntries(dataHistogram->GetEntries());
  }
  
  TH1D* createMirroredHistogram(TH1D* histogram)
  {
    QString title = histogram->GetTitle();
    QString name = QString(histogram->GetName()) + "_mirrored";
    QVector<double> binning;
    int nBins = histogram->GetNbinsX();
    for (int i = 0; i <= nBins; ++i) {
      double lowEdge = histogram->GetBinLowEdge(i+1);
      lowEdge *= -1;
      binning.prepend(lowEdge);
    }
    TH1D* mirroredHistogram = new TH1D(qPrintable(title), qPrintable(name), nBins, binning.constData());
    mirroredHistogram->Sumw2();
    mirroredHistogram->SetMarkerColor(histogram->GetMarkerColor());
    mirroredHistogram->SetMarkerSize(histogram->GetMarkerSize());
    mirroredHistogram->SetMarkerStyle(histogram->GetMarkerStyle());
    mirroredHistogram->SetLineColor(histogram->GetLineColor());
    mirroredHistogram->SetLineWidth(histogram->GetLineWidth());
    mirroredHistogram->SetLineStyle(histogram->GetLineStyle());
    mirroredHistogram->SetXTitle(qPrintable(QString("- ")+histogram->GetXaxis()->GetTitle()));
    updateMirroredHistogram(mirroredHistogram, histogram);
    
    return mirroredHistogram;
  }

  QString analysisPath()
  {
    const char* env = getenv("PERDAIXANA_PATH");
    if (!env)
      qFatal("ERROR: You need to set PERDAIXANA_PATH environment variable to the toplevel location!");
    return env;
  }

  QString dataPath()
  {
    const char* env = getenv("PERDAIXDATA_PATH");
    if (!env)
      qFatal("ERROR: You need to set PERDAIXDATA_PATH environment variable to the toplevel location of the data!");
    return env;
  }
  
  void scaleToBinWidth(TH1D* histogram)
  {
    if (!histogram->GetSumw2())
      histogram->Sumw2();
    histogram->Scale(1., "width");
  }
  
  TGraphAsymmErrors* createBeltWithSystematicUncertainty(const TH1D* histogram,const TH1D* lowerHistogram,const TH1D* upperHistogram)
  {
    QList<double> x;
    QList<double> exl;
    QList<double> exu;
    QList<double> y;
    QList<double> eyl;
    QList<double> eyu;
    for(int i = 1; i <= histogram->GetNbinsX(); i++) {
      x.push_back(histogram->GetBinCenter(i));
      exl.push_back(histogram->GetBinCenter(i) - histogram->GetBinLowEdge(i));
      exu.push_back( (histogram->GetBinLowEdge(i) + histogram->GetBinWidth(i) ) - histogram->GetBinCenter (i));
      QVector<double> point;
      point << lowerHistogram->GetBinContent(i) << histogram->GetBinContent(i) <<upperHistogram->GetBinContent(i);
      qSort(point);
      const double min = point[0];
      const double middle = point[1];
      const double max = point[2];
      y.push_back(middle);
      eyl.push_back(middle - min);
      eyu.push_back(max - middle);
    }
    TGraphAsymmErrors* graph = new TGraphAsymmErrors(x.size(), &(x.toVector().toStdVector()[0]), &(y.toVector().toStdVector()[0]), &(exl.toVector().toStdVector()[0]), &(exu.toVector().toStdVector()[0]), &(eyl.toVector().toStdVector()[0]), &(eyu.toVector().toStdVector()[0]));
    QString name = QString(histogram->GetName()) + "_graph";
    graph->SetName(name.toStdString().c_str());
    return graph;
  }
}
