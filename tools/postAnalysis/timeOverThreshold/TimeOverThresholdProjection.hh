#ifndef TimeOverThresholdProjection_hh
#define TimeOverThresholdProjection_hh

#include "PostAnalysisPlot.hh"
#include "H1DPlot.hh"

class PostAnalysisCanvas;

class TimeOverThresholdProjection : public PostAnalysisPlot, public H1DPlot {
public:
  TimeOverThresholdProjection(PostAnalysisCanvas*, int ch);
  virtual ~TimeOverThresholdProjection();
};

#endif
