#ifndef AzimuthMigrationHistogram_hh
#define AzimuthMigrationHistogram_hh

#include "AnalysisPlot.hh"
#include "H2DPlot.hh"

#include <TVector3.h>

class AzimuthMigrationHistogram : public AnalysisPlot, public H2DPlot
{
public:
  AzimuthMigrationHistogram();
  ~AzimuthMigrationHistogram();
  void processEvent(const AnalyzedEvent*);
};

#endif /* AzimuthMigrationHistogram_hh */
