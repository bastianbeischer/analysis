#ifndef MCRIGIDITYRESOLUTION_HH
#define MCRIGIDITYRESOLUTION_HH

#include "AnalysisPlot.hh"
#include "H1DPlot.hh"

#include <QMap>
#include <QPair>

class ParticleProperties;

class MCRigidityResolution : public AnalysisPlot, public H1DPlot
{
public:
  MCRigidityResolution(int pdgID = 11);
  ~MCRigidityResolution();

  virtual void processEvent(const QVector<Hit*>&, Particle* = 0, SimpleEvent* = 0);
  virtual void update();
  virtual void finalize();
  void saveHistos();
private:
  const ParticleProperties* m_particle;
  const double m_rigidityRangeLower;
  const double m_rigidityRangeUppper;
  const int m_numberOfBins;


  QMap < int, TH1D*> m_resolutionHistos;
};

#endif // MCRIGIDITYRESOLUTION_HH
