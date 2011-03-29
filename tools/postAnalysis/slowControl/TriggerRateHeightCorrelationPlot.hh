#ifndef TriggerRateHeightCorrelationPlot_hh
#define TriggerRateHeightCorrelationPlot_hh

#include "PostAnalysisPlot.hh"
#include "H1DPlot.hh"

class PostAnalysisCanvas;

class TriggerRateHeightCorrelationPlot : public PostAnalysisPlot, public H1DPlot {
public:
  TriggerRateHeightCorrelationPlot(PostAnalysisCanvas*, int ch);
  virtual ~TriggerRateHeightCorrelationPlot();
};

#endif
