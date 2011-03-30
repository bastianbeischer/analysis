#ifndef TriggerRateHeightTimePlot_hh
#define TriggerRateHeightTimePlot_hh

#include "PostAnalysisPlot.hh"
#include "H1DPlot.hh"

class PostAnalysisCanvas;
class TriggerRateTimePlot;
class HeightTimePlot;

class TriggerRateHeightTimePlot : public PostAnalysisPlot, public H1DPlot {
public:
  TriggerRateHeightTimePlot(TriggerRateTimePlot*, HeightTimePlot*);
  virtual ~TriggerRateHeightTimePlot();
  virtual void draw(TCanvas* canvas);
};

#endif
