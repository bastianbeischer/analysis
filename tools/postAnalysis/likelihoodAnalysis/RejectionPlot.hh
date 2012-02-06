#ifndef RejectionPlot_hh
#define RejectionPlot_hh

#include "PostAnalysisPlot.hh"
#include "H1DPlot.hh"

class LogLikelihoodProjectionPlot;

class RejectionPlot : public PostAnalysisPlot, public H1DPlot {
public:
  RejectionPlot(LogLikelihoodProjectionPlot*);
  ~RejectionPlot();
};

#endif
