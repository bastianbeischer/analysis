#include "Helpers.hh"
#include "ParticleProperties.hh"
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

  double trackerResolution(double curvature, Enums::Particle particle)
  {
    ParticleProperties properties(particle);
    Q_ASSERT(properties.charge() != 0 && properties.mass() > 0);
    double a = 0, b = 0;
    switch (particle) {
      case Enums::Proton: case Enums::AntiProton: a = 0.077; b = 0.3175; break;
      case Enums::Electron: case Enums::Positron: a = 0.07627; b = 0.2349; break;
      case Enums::Helium: a = 0.04195; b = 0.3024; break;
      case Enums::PiPlus: case Enums::PiMinus: a = 0.0759; b = 0.2356; break;
      default: a = 0.077; b = 0.3175; break;
    }
    double inverseBeta = addQuad(1, properties.mass() * curvature);
    return addQuad(a / curvature, b * inverseBeta);
  }

  double tofResolution()
  {
    double l = Constants::upperTofPosition - Constants::lowerTofPosition;
    return Constants::tofResolution * Constants::speedOfLight / l;
  }
}
