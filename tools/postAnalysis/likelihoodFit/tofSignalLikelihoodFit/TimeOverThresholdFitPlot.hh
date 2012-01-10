#ifndef TimeOverThresholdFitPlot_hh
#define TimeOverThresholdFitPlot_hh

#include "LikelihoodPDFFitPlot.hh"

class TimeOverThresholdFitPlot : public LikelihoodPDFFitPlot
{
public:
  TimeOverThresholdFitPlot(Likelihood*, const TH2D*, int);
};

#endif
