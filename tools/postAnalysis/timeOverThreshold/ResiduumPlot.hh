#ifndef ResiduumPlot_hh
#define ResiduumPlot_hh

#include "PostAnalysisPlot.hh"
#include "H1DPlot.hh"

class ResiduumPlot : public PostAnalysisPlot, public H1DPlot {
public:
  ResiduumPlot(TCanvas* canvas, QString title, double referenceValue);
  virtual ~ResiduumPlot();
};

#endif
