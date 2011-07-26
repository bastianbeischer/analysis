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
  void processEvent(const QVector<Hit*>&, const Particle* const = 0, const SimpleEvent* const = 0);

};

#endif /* AzimuthMigrationHistogram_hh */
