#ifndef PMTPlot_hh
#define PMTPlot_hh

#include "AnalysisPlot.hh"
#include "H1DPlot.hh"

#include "SensorTypes.hh"

class PMTPlot :
  public AnalysisPlot,
  public H1DPlot
{

public:
  PMTPlot(SensorTypes::Type);
  ~PMTPlot();
  
  void processEvent(const QVector<Hit*>&, Particle* = 0, SimpleEvent* = 0);

private:
  SensorTypes::Type m_type;

};

#endif /* PMTPlot_hh */
