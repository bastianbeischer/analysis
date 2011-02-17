#ifndef TrackingEfficiencyVsMomentumPlot_hh
#define TrackingEfficiencyVsMomentumPlot_hh

#include "AnalysisPlot.hh"
#include "H2DPlot.hh"

class TCanvas;

class TrackingEfficiencyVsMomentumPlot :
  public AnalysisPlot,
  public H2DPlot
{
  
public:
  TrackingEfficiencyVsMomentumPlot();
  ~TrackingEfficiencyVsMomentumPlot();
  
  void processEvent(const QVector<Hit*>&, Track* = 0, SimpleEvent* = 0);
  void draw(TCanvas*);
  void finalize();

};

#endif /* TrackingEfficiencyVsMomentumPlot_hh */
