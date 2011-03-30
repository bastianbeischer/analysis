#ifndef TriggerRateHeightCorrelationPlot_hh
#define TriggerRateHeightCorrelationPlot_hh

#include "PostAnalysisPlot.hh"
#include "GraphPlot.hh"

class TriggerRateTimePlot;
class HeightTimePlot;

class TriggerRateHeightCorrelationPlot : public PostAnalysisPlot, public GraphPlot {
public:
  TriggerRateHeightCorrelationPlot(TriggerRateTimePlot*, HeightTimePlot*);
  virtual ~TriggerRateHeightCorrelationPlot();
};

#endif
