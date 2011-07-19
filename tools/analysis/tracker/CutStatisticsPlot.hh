#ifndef CutStatisticsPlot_hh
#define CutStatisticsPlot_hh

#include "AnalysisPlot.hh"
#include "H1DPlot.hh"

class CutStatisticsPlot :
  public AnalysisPlot,
  public H1DPlot
{
  
public:
  CutStatisticsPlot();
  ~CutStatisticsPlot();
  
  void processEvent(const QVector<Hit*>&, const Particle* const = 0, const SimpleEvent* const = 0);
  void update();
  void finalize();

private:
  
};

#endif /* CutStatisticsPlot_hh */
