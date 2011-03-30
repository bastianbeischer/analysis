#ifndef HeightTimePlot_hh
#define HeightTimePlot_hh

#include "PostAnalysisPlot.hh"
#include "H1DPlot.hh"

class PostAnalysisCanvas;
class TriggerRateTimePlot;
class HeightTimePlot;
class TCanvas;

class HeightTimePlot : public PostAnalysisPlot, public H1DPlot {
public:
  HeightTimePlot(PostAnalysisCanvas*);
  virtual ~HeightTimePlot();
  virtual void draw(TCanvas*);
};

#endif
