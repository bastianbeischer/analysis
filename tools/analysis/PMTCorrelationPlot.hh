#ifndef PMTCorrelationPlot_hh
#define PMTCorrelationPlot_hh

#include "AnalysisPlot.hh"
#include "H2DProjectionPlot.hh"

#include "SensorTypes.hh"

class PMTCorrelationPlot :
  public AnalysisPlot,
  public H2DProjectionPlot
{

public:
  PMTCorrelationPlot();
  ~PMTCorrelationPlot();
  
  void processEvent(const QVector<Hit*>&, Particle* = 0, SimpleEvent* = 0);

};

#endif /* PMTCorrelationPlot_hh */
