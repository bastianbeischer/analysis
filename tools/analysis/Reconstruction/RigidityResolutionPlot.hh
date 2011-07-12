#ifndef RigidityResolutionPlot_hh
#define RigidityResolutionPlot_hh

#include "AnalysisPlot.hh"
#include "H1DPlot.hh"

#include "Particle.hh"

#include <QMap>
#include <QPair>

class ParticleProperties;
class RootQtWidget;

class RigidityResolutionPlot : public AnalysisPlot, public H1DPlot
{
public:
  RigidityResolutionPlot(AnalysisPlot::Topic, const Particle::Type& = Particle::Electron);
  ~RigidityResolutionPlot();

  virtual void processEvent(const QVector<Hit*>&, Particle* = 0, SimpleEvent* = 0);
  void update();
  void finalize();
  void positionChanged(double, double);

private:
  virtual double referenceRigidity(SimpleEvent*) const = 0;
  void loadRigHisto(double rig);
  void loadRigHisto(int bin);
  void saveHistos();

protected:
  const ParticleProperties* m_particle;
  const double m_rigidityRangeLower;
  const double m_rigidityRangeUppper;
  const int m_numberOfBins;

  RootQtWidget* m_rigDistributionWidget;

  QMap<int, TH1D*> m_resolutionHistos;

};

#endif // RigidityResolutionPlot_hh
