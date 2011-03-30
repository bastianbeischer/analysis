#ifndef TriggerRateHeightTimePlot_hh
#define TriggerRateHeightTimePlot_hh

#include "PostAnalysisPlot.hh"
#include "H1DPlot.hh"

class PostAnalysisCanvas;
class TriggerRateTimePlot;
class HeightTimePlot;
class TGraph;

class TriggerRateHeightTimePlot : public PostAnalysisPlot, public H1DPlot {
public:
  TriggerRateHeightTimePlot(PostAnalysisCanvas* trigger, PostAnalysisCanvas* height);
  virtual ~TriggerRateHeightTimePlot();
  virtual void draw(TCanvas* canvas);
private:
  TGraph* m_graph;
};

#endif
