#ifndef MeasurementTimeDistributionPlot_hh
#define MeasurementTimeDistributionPlot_hh

#include "AnalysisPlot.hh"
#include "H1DPlot.hh"
#include "MeasurementTimeCalculation.hh"

class MeasurementTimeDistributionPlot : public AnalysisPlot, public H1DPlot {
public:
  MeasurementTimeDistributionPlot(int numberOfThreads);
  virtual ~MeasurementTimeDistributionPlot();
  void processEvent(const AnalyzedEvent*);
  virtual void update();
private:
  MeasurementTimeCalculation* m_calculation;
};

#endif
