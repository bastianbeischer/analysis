#include "Helpers.hh"

#include <cmath>

#include <QVector>

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
}
