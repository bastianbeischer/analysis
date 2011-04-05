#ifndef TriggerRateHeightCorrelationPlot_hh
#define TriggerRateHeightCorrelationPlot_hh

#include "PostAnalysisPlot.hh"
#include "GraphPlot.hh"

class PostAnalysisCanvas;

class TriggerRateHeightCorrelationPlot : public PostAnalysisPlot, public GraphPlot {
public:
  TriggerRateHeightCorrelationPlot(PostAnalysisCanvas* trigger, PostAnalysisCanvas* height);
  virtual ~TriggerRateHeightCorrelationPlot();
};

#endif
