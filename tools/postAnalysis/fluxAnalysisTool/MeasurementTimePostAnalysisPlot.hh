#ifndef MeasurementTimePostAnalysisPlot_hh
#define MeasurementTimePostAnalysisPlot_hh

#include "PostAnalysisCanvas.hh"
#include "PostAnalysisPlot.hh"
#include "H1DPlot.hh"
#include "MeasurementTimeCalculation.hh"

#include <TH1D.h>

class MeasurementTimePostAnalysisPlot : public PostAnalysisPlot, public H1DPlot {
public:
  MeasurementTimePostAnalysisPlot(PostAnalysisCanvas*);
  virtual ~MeasurementTimePostAnalysisPlot();
  double measurementTime();
private:
  MeasurementTimeCalculation* m_timeCalculation;
};

#endif
