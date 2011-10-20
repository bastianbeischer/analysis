#ifndef AzimuthDistributionPlot_hh
#define AzimuthDistributionPlot_hh

#include "AnalysisPlot.hh"
#include "H1DPlot.hh"

class AzimuthDistributionPlot :
  public AnalysisPlot,
  public H1DPlot
{
public:
  AzimuthDistributionPlot();
  ~AzimuthDistributionPlot();
  void processEvent(const AnalyzedEvent*);
};

#endif /* AzimuthDistributionPlot_hh */
