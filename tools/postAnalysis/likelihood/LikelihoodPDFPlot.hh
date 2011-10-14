#ifndef LikelihoodPDFPlot_hh
#define LikelihoodPDFPlot_hh

#include "PostAnalysisPlot.hh"
#include "H1DPlot.hh"

class Likelihood;

class LikelihoodPDFPlot : public PostAnalysisPlot, public H1DPlot
{
public:
  LikelihoodPDFPlot(Likelihood* likelihood, double momentum);
  ~LikelihoodPDFPlot();
};

#endif
