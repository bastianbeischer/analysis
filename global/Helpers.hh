#ifndef Helpers_hh
#define Helpers_hh

#include <QVector>

#include <TH1D.h>

namespace Helpers
{
  double addQuad(double a, double b);
  QVector<double> logBinning(unsigned int nBins, double min, double max);
  TH1D* createMirroredHistogram(TH1D* histogram);
  void updateMirroredHistogram(TH1D* histogramToUpdate, TH1D* dataHistogram);
}

#endif
