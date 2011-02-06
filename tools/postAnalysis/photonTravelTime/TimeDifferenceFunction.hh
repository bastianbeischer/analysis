#ifndef TimeDifferenceFunction_hh
#define TimeDifferenceFunction_hh

#include "PostAnalysisPlot.hh"
#include "H2DPlot.hh"

class TF2;

class TimeDifferenceFunction : public PostAnalysisPlot, public H2DPlot {
public:
  TimeDifferenceFunction(TCanvas*);
  virtual ~TimeDifferenceFunction();
  TF2* function();
private:
  TF2* m_function;
};

#endif
