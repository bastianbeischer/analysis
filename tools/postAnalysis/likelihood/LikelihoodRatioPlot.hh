#ifndef LikelihoodRatioPlot_hh
#define LikelihoodRatioPlot_hh

#include "PostAnalysisPlot.hh"
#include "H1DPlot.hh"

class Likelihood;

class LikelihoodRatioPlot : public PostAnalysisPlot, public H1DPlot
{
public:
  LikelihoodRatioPlot(Likelihood* likelihood, double momentum);
  ~LikelihoodRatioPlot();
};

#endif
