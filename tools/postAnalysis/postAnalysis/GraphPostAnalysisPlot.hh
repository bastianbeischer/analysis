#ifndef GraphPostAnalysisPlot_hh
#define GraphPostAnalysisPlot_hh

#include "PostAnalysisPlot.hh"
#include "GraphPlot.hh"

#include <QString>

#include <TGraph.h>

class GraphPostAnalysisPlot : public PostAnalysisPlot, public GraphPlot {
public:
  GraphPostAnalysisPlot(TGraph* graph);
  GraphPostAnalysisPlot(TGraph* graph1, TGraph* graph2);
  virtual ~GraphPostAnalysisPlot();

private:

};

#endif
