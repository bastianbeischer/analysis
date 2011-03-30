#ifndef TrackerTemperaturePlot_hh
#define TrackerTemperaturePlot_hh

#include "PostAnalysisPlot.hh"
#include "H1DPlot.hh"

class PostAnalysisCanvas;
class TriggerRateTimePlot;
class TrackerTemperaturePlot;
class TCanvas;

class TrackerTemperaturePlot : public PostAnalysisPlot, public H1DPlot {
public:
  TrackerTemperaturePlot(const QVector<PostAnalysisCanvas*>);
  virtual ~TrackerTemperaturePlot();
  virtual void draw(TCanvas*);
private:
  int layer(double z);
};

#endif
