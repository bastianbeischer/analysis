#ifndef TrackerTemperaturePlot_hh
#define TrackerTemperaturePlot_hh

#include "PostAnalysisPlot.hh"
#include "H1DPlot.hh"

class PostAnalysisCanvas;

class TrackerTemperaturePlot : public PostAnalysisPlot, public H1DPlot {
public:
  TrackerTemperaturePlot(PostAnalysisCanvas*, int ch);
  virtual ~TrackerTemperaturePlot();
};

#endif
