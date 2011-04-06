#ifndef TOFTimeShiftTriggerPlot_hh
#define TOFTimeShiftTriggerPlot_hh

#include "AnalysisPlot.hh"
#include "H2DPlot.hh"

class TOFTimeShiftTriggerPlot : public AnalysisPlot, public H2DPlot {
public:
  TOFTimeShiftTriggerPlot();
  virtual ~TOFTimeShiftTriggerPlot();
  virtual void processEvent(const QVector<Hit*>&, Track* = 0, SimpleEvent* = 0);
  void draw(TCanvas* canvas);
};

#endif
