#ifndef ParameterPlot_hh
#define ParameterPlot_hh

#include "PostAnalysisPlot.hh"
#include "GraphPlot.hh"
#include "Enums.hh"

class Likelihood;

class ParameterPlot : public PostAnalysisPlot, public GraphPlot {
public:
  ParameterPlot(Likelihood* likelihood, Enums::Particle);
  ~ParameterPlot();
};

#endif
