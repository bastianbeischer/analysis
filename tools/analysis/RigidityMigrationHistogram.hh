#ifndef RigidityMigrationHistogram_hh
#define RigidityMigrationHistogram_hh

#include "MCSimpleEventParticle.hh"
#include "AnalysisPlot.hh"
#include "H2DPlot.hh"

#include <QVector>

class RigidityMigrationHistogram : public AnalysisPlot, public H2DPlot
{
public:
  RigidityMigrationHistogram();
  ~RigidityMigrationHistogram();
  void processEvent(const QVector<Hit*>&, const Particle* const = 0, const SimpleEvent* const = 0);
};

#endif /* RigidityMigrationHistogram_hh */
