#ifndef TimeDifferencePlot_hh
#define TimeDifferencePlot_hh

#include "PostAnalysisPlot.hh"
#include "H2DPlot.hh"

class PostAnalysisCanvas;
class TimeDifferenceFunction;

class TimeDifferencePlot : public PostAnalysisPlot, public H2DPlot {
public:
  TimeDifferencePlot(PostAnalysisCanvas*, TimeDifferenceFunction*);
  virtual ~TimeDifferencePlot();
  virtual void draw(TCanvas*);
};

#endif
