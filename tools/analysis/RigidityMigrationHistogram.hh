#ifndef RigidityMigrationHistogram_hh
#define RigidityMigrationHistogram_hh

#include "MCSimpleEventParticle.hh"
#include "AnalysisPlot.hh"
#include "H2DPlot.hh"

#include <TVector3.h>
#include <TRandom3.h>

#include <QVector>

class RigidityMigrationHistogram : public AnalysisPlot, public H2DPlot
{
public:
  RigidityMigrationHistogram();
  ~RigidityMigrationHistogram();
  void processEvent(const QVector<Hit*>&, const Particle* const = 0, const SimpleEvent* const = 0);
private:
  TRandom3* m_random;
  double smearedRigidity(const MCSimpleEventParticle* mcParticle, TRandom3* ran, double factor = 1);
  double smearedMomentum(const MCSimpleEventParticle* mcParticle, TRandom3* ran, double factor = 1);
  double perdaixMomentumResolution(double momentum, double beta, double factor = 1);
  double betaFromMc(const MCSimpleEventParticle* mcParticle);
};

#endif /* RigidityMigrationHistogram_hh */
