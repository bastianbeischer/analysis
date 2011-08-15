#ifndef TimeResolutionPlot_hh
#define TimeResolutionPlot_hh

#include "PostAnalysisPlot.hh"
#include "GraphPlot.hh"
#include <TMatrixT.h>
#include <QMap>

class TimeOfFlightHistogram;

class TimeResolutionPlot : public PostAnalysisPlot, public GraphPlot {
public:
  TimeResolutionPlot(const QVector<TimeOfFlightHistogram*>&, int nBins);
  virtual ~TimeResolutionPlot();
private:
  struct Key {
    Key(int vi, int vj, int vk, int vl);
    Key(const TimeOfFlightHistogram* const);
    bool operator<(const Key& other) const;
    int i, j, k, l;
  };
  struct Value {
    Value(double, double);
    double v, vError;
  };
  
  double sumSigmaErrorJL(const QMap<Key, Value>& map, int i, int k);
  double sumSigmaErrorIK(const QMap<Key, Value>& map, int j, int l);
  double sumRelativeSigmaError(const QMap<Key, Value>& map, int i, int j, int k, int l);
  double sumRelativeSigmaErrorJL(const QMap<Key, Value>& map, int i, int k);
  double sumRelativeSigmaErrorIK(const QMap<Key, Value>& map, int j, int l);
  void dumpMatrix(const TMatrixT<double>& m);

  int m_nBins;
};

#endif
