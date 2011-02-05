#ifndef TotalSignalHeightPlot_hh
#define TotalSignalHeightPlot_hh

#include "AnalysisPlot.hh"
#include "H1DPlot.hh"

class TotalSignalHeightPlot :
  public AnalysisPlot,
  public H1DPlot
{
  
public:
  TotalSignalHeightPlot();
  ~TotalSignalHeightPlot();
  
  void processEvent(const QVector<Hit*>&, Track* = 0, SimpleEvent* = 0);
  
};

#endif /* TotalSignalHeightPlot_hh */
