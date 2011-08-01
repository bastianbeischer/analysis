#ifndef AzimuthCutStatistics_hh
#define AzimuthCutStatistics_hh

#include "AnalysisPlot.hh"
#include "H2DProjectionPlot.hh"

class AzimuthCutStatistics :
  public AnalysisPlot,
  public H2DProjectionPlot
{
public:
  AzimuthCutStatistics();
  ~AzimuthCutStatistics();
  void processEvent(const QVector<Hit*>&, const Particle* const = 0, const SimpleEvent* const = 0);
};

#endif /* AzimuthCutStatistics_hh */
