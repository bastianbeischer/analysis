#ifndef TimeResolutionAnalysis_hh
#define TimeResolutionAnalysis_hh

#include <QMap>
#include <QVector>
#include <TMatrixT.h>

class TimeOfFlightHistogram;

class TimeResolutionAnalysis {
public:
  TimeResolutionAnalysis(const QVector<TimeOfFlightHistogram*>&, int nBins);
  virtual ~TimeResolutionAnalysis();

  int nBins() const;
  double y(int) const;
  double vIK(int, int, double r = -1.) const;
  double vJL(int, int, double r = -1.) const;
  double sigmaIK(int, int, double r = -1.) const;
  double sigmaJL(int, int, double r = -1.) const;
private:
  struct Key {
    Key(int vi, int vj, int vk, int vl);
    Key(const TimeOfFlightHistogram* const);
    bool operator<(const Key& other) const;
    int i, j, k, l;
  };
  struct Value {
    Value(double, double, double, double);
    double v, vError;
    double upperY, lowerY;
  };

  double sumSigmaErrorJL(const QMap<Key, Value>& map, int i, int k);
  double sumSigmaErrorIK(const QMap<Key, Value>& map, int j, int l);
  double sumRelativeSigmaError(const QMap<Key, Value>& map, int i, int j, int k, int l);
  double sumRelativeSigmaErrorJL(const QMap<Key, Value>& map, int i, int k);
  double sumRelativeSigmaErrorIK(const QMap<Key, Value>& map, int j, int l);
  void dumpMatrix(const TMatrixT<double>& m);

  int m_nBins;
  double m_r;
  QMap<int, double> m_y;
  QVector<double> m_variance;
  QVector<double> m_varianceCorrection;
};

#endif
