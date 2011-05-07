#ifndef MCRIGIDITYRESOLUTION_HH
#define MCRIGIDITYRESOLUTION_HH

#include "AnalysisPlot.hh"
#include "H1DPlot.hh"

#include <QMap>
#include <QPair>

class ParticleProperties;
class RootQtWidget;
class QObject;

class MCRigidityResolution : public AnalysisPlot, public H1DPlot, public QObject
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

  void setupSecondaryWidget();

  RootQtWidget* m_rigDistributionWidget;

  QMap < int, TH1D*> m_resolutionHistos;

public slots:
  void loadRigHisto(double rig);
  void loadRigHisto(int bin);
};

#endif // MCRIGIDITYRESOLUTION_HH
