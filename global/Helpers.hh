#ifndef Helpers_hh
#define Helpers_hh

#include <cmath>

#include <QVector>

namespace Helpers
{
  double addQuad(double a, double b, double c = 0, double d = 0, double e = 0);
  QVector<double> logBinning(unsigned int nBins, double min, double max);
}

#endif
