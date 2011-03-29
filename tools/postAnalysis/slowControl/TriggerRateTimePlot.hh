#ifndef TriggerRateTimePlot_hh
#define TriggerRateTimePlot_hh

#include "PostAnalysisPlot.hh"
#include "H2DPlot.hh"

class PostAnalysisCanvas;

class TriggerRateTimePlot : public PostAnalysisPlot, public H2DPlot {
public:
  TriggerRateTimePlot(PostAnalysisCanvas*);
  virtual ~TriggerRateTimePlot();
};

#endif
