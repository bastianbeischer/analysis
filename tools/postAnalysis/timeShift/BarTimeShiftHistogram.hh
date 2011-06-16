#ifndef BarTimeShiftHistogram_hh
#define BarTimeShiftHistogram_hh

#include "PostAnalysisPlot.hh"
#include "H2DPlot.hh"

#include <TMatrixT.h>

class BarShiftPlot;

class BarTimeShiftHistogram : public PostAnalysisPlot, public H2DPlot {
public:
  BarTimeShiftHistogram(const QVector<BarShiftPlot*>&);
  virtual ~BarTimeShiftHistogram();
private:
  double desiredTimeDifference(int barPositionDistance);
  void align(const QVector<BarShiftPlot*>&);
  void dumpMatrix(const TMatrixT<double>&);
};

#endif
