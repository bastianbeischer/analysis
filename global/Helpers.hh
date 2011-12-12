#ifndef Helpers_hh
#define Helpers_hh

#include "Enums.hh"

#include <QVector>
#include <QString>

class TH1D;
class TGraphAsymmErrors;

namespace Helpers
{
  double addQuad(double a, double b, double c = 0, double d = 0, double e = 0);
  QVector<double> logBinning(unsigned int nBins, double min, double max);
  TH1D* createMirroredHistogram(TH1D* histogram);
  void updateMirroredHistogram(TH1D* histogramToUpdate, TH1D* dataHistogram);
  QString analysisPath();
  QString dataPath();
  double trackerResolution(double curvature, Enums::Particle = Enums::NoParticle); // sigma_K / K, curvature K
  double tofResolution();
  void scaleToBinWidth(TH1D*);
  TGraphAsymmErrors* createBeltWithSystematicUncertainty(const TH1D* histogram,const TH1D* lowerHistogram,const TH1D* upperHistogram);
}

#endif
