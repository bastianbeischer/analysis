#ifndef ZenithDistributionPlot_hh
#define ZenithDistributionPlot_hh

#include "AnalysisPlot.hh"
#include "H1DPlot.hh"

class ZenithDistributionPlot :
  public AnalysisPlot,
  public H1DPlot
{
public:
  ZenithDistributionPlot();
  ~ZenithDistributionPlot();
  
  void processEvent(const QVector<Hit*>&, const Particle* const = 0, const SimpleEvent* const = 0);
  void update();

private:

};

#endif /* ZenithDistributionPlot_hh */
