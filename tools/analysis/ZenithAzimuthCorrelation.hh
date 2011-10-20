#ifndef ZenithAzimuthCorrelation_hh
#define ZenithAzimuthCorrelation_hh

#include "AnalysisPlot.hh"
#include "H2DPlot.hh"

class ZenithAzimuthCorrelation : public AnalysisPlot, public H2DPlot {
public:
  ZenithAzimuthCorrelation();
  ~ZenithAzimuthCorrelation();
  void processEvent(const AnalyzedEvent*);
  void update();
};

#endif /* ZenithAzimuthCorrelation_hh */
