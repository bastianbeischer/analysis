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
  
  void processEvent(const AnalyzedEvent*);
  void update();
  void finalize();

};

#endif /* TrackerLayerStatisticsPlot_hh */
