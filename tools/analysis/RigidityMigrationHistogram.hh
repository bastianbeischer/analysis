#ifndef RigidityMigrationHistogram_hh
#define RigidityMigrationHistogram_hh

#include "AnalysisPlot.hh"
#include "H2DPlot.hh"

#include <TVector3.h>

#include <QVector>

class RigidityMigrationHistogram : public AnalysisPlot, public H2DPlot
{
public:
  enum Type {All, Positive, Negative};
  RigidityMigrationHistogram(Type type);
  ~RigidityMigrationHistogram();
  void processEvent(const QVector<Hit*>&, const Particle* const = 0, const SimpleEvent* const = 0);
private:
  Type m_type;
};

#endif /* RigidityMigrationHistogram_hh */
