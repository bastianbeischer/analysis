#ifndef EfficiencyPlot_hh
#define EfficiencyPlot_hh

#include "PostAnalysisPlot.hh"
#include "H1DPlot.hh"

class LogLikelihoodProjectionPlot;

class EfficiencyPlot : public PostAnalysisPlot, public H1DPlot {
public:
  EfficiencyPlot(LogLikelihoodProjectionPlot*);
  ~EfficiencyPlot();
};

#endif
