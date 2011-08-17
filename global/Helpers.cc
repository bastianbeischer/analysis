#include "Helpers.hh"

#include <cmath>

#include <QVector>
#include <QString>

namespace Helpers
{
  double addQuad(double a, double b) {return sqrt(a*a + b*b);}

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
    QString name = QString(histogram->GetName())+"_mirrored";
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
}
