#ifndef AzimuthCutStatistics_hh
#define AzimuthCutStatistics_hh

#include "AnalysisPlot.hh"
#include "H2DProjectionPlot.hh"

class AzimuthCutStatistics : public AnalysisPlot, public H2DProjectionPlot
{
public:
  AzimuthCutStatistics();
  ~AzimuthCutStatistics();
  void processEvent(const AnalyzedEvent*);
};

#endif /* AzimuthCutStatistics_hh */
