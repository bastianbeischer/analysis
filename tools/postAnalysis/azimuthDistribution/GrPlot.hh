#ifndef GrPlot_hh
#define GrPlot_hh

#include "PostAnalysisCanvas.hh"
#include "PostAnalysisPlot.hh"
#include "GraphPlot.hh"

#include <QString>

#include <TGraph.h>

class GrPlot : public PostAnalysisPlot, public GraphPlot {
public:
  GrPlot(TGraph* graph);
  virtual ~GrPlot();

private:

};

#endif
