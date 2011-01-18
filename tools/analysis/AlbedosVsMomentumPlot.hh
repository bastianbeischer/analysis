#ifndef AlbedosVsMomentumPlot_hh
#define AlbedosVsMomentumPlot_hh

#include "AnalysisPlot.hh"
#include "H1DPlot.hh"

class AlbedosVsMomentumPlot :
  public AnalysisPlot,
  public H1DPlot
{
  
public:
  AlbedosVsMomentumPlot();
  ~AlbedosVsMomentumPlot();
  
  void processEvent(const QVector<Hit*>&, Track* = 0, SimpleEvent* = 0);
  // void update();

};

#endif /* AlbedosVsMomentumPlot_hh */
