#ifndef Helpers_hh
#define Helpers_hh

#include <cmath>

#include <QVector>
#include <QString>

namespace Helpers
{
  double addQuad(double a, double b);
  QVector<double> logBinning(unsigned int nBins, double min, double max);
  QString analysisPath();
  QString dataPath();
}

#endif
