#include "Helpers.hh"

#include "Constants.hh"

#include <cmath>

#include <QVector>
#include <QString>

#include <TH1.h>

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
  
  double idealTrackerSignalHeight(Enums::Situation situation)
  {
    switch (situation) {
      case Enums::KirunaFlight:
        return Constants::idealTrackerSignalHeightFlight;
      case Enums::KirunaMuons:
        return Constants::idealTrackerSignalHeightGround;
      default:
        qDebug("Warning: Reference value for this situation is not set yet.");
        return 0;
    }
  }
}
