#ifndef BarTimeShiftHistogram_hh
#define BarTimeShiftHistogram_hh

#include "PostAnalysisPlot.hh"
#include "H2DPlot.hh"

class BarShiftPlot;

class BarTimeShiftHistogram : public PostAnalysisPlot, public H2DPlot {
public:
  BarTimeShiftHistogram(const QVector<BarShiftPlot*>&);
  virtual ~BarTimeShiftHistogram();
};

#endif
