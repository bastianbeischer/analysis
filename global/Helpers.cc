#include "Helpers.hh"

#include <cmath>

#include <QVector>

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
}
