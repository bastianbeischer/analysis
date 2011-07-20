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
  
  void processEvent(const QVector<Hit*>&, const Particle* const = 0, const SimpleEvent* const = 0);
  
};

#endif /* TotalSignalHeightPlot_hh */
