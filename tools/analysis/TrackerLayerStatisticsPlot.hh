#ifndef TrackerLayerStatisticsPlot_hh
#define TrackerLayerStatisticsPlot_hh

#include "AnalysisPlot.hh"
#include "H1DPlot.hh"

class TrackerLayerStatisticsPlot :
  public AnalysisPlot,
  public H1DPlot
{
  
public:
  TrackerLayerStatisticsPlot();
  ~TrackerLayerStatisticsPlot();
  
  void processEvent(const QVector<Hit*>&, Track* = 0, SimpleEvent* = 0);
  void update();
  void finalize();

};

#endif /* TrackerLayerStatisticsPlot_hh */
