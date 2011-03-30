#ifndef TriggerRateTimePlot_hh
#define TriggerRateTimePlot_hh

#include "PostAnalysisPlot.hh"
#include "H1DPlot.hh"

class PostAnalysisCanvas;

class TriggerRateTimePlot : public PostAnalysisPlot, public H1DPlot {
public:
  TriggerRateTimePlot(PostAnalysisCanvas*);
  virtual ~TriggerRateTimePlot();
};

#endif
