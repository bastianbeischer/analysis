#ifndef TimeOverThresholdProjection_hh
#define TimeOverThresholdProjection_hh

#include "PostAnalysisPlot.hh"
#include "H1DPlot.hh"

class TimeOverThresholdProjection : public PostAnalysisPlot, public H1DPlot {
public:
  TimeOverThresholdProjection(TCanvas*, int ch);
  virtual ~TimeOverThresholdProjection();
};

#endif
