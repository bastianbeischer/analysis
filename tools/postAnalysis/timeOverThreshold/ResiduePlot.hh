#ifndef ResiduePlot_hh
#define ResiduePlot_hh

#include "PostAnalysisPlot.hh"
#include "H1DPlot.hh"

class PostAnalysisCanvas;

class ResiduePlot : public PostAnalysisPlot, public H1DPlot {
public:
  ResiduePlot(PostAnalysisCanvas* canvas);
  virtual ~ResiduePlot();
private:
  const static double s_reference;
};

#endif
