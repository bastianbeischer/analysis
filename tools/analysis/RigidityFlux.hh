#ifndef RigidityFlux_hh
#define RigidityFlux_hh

#include "H1DPlot.hh"
#include "AnalysisPlot.hh"
#include "FluxCalculation.hh"
#include "MeasurementTimeCalculation.hh"
#include "EfficiencyCorrectionSettings.hh"
#include "SolarModulationFit.hh"
#include "Plotter.hh"

#include <TH1D.h>

#include <QString>
#include <QDateTime>
#include <QVector>
#include <QMap>

class Hit;
class Particle;
class SimpleEvent;
class TF1;

class RigidityFlux : public QObject, public AnalysisPlot, public H1DPlot {
Q_OBJECT
public:
  enum Type {Positive, Negative};
  RigidityFlux(Type type, const QDateTime& first, const QDateTime& last, TH1D* particleHistogram);
  ~RigidityFlux();

  virtual void processEvent(const QVector<Hit*>&, const Particle* const = 0, const SimpleEvent* const = 0);
  virtual void finalize();
  virtual void update();
private slots:
  void selectionChanged();

private:
  void loadEfficiencies();
  void efficiencyCorrection();
  void updateBinTitles();
  Type m_type;
  EfficiencyCorrectionSettings* m_effCor;
  MeasurementTimeCalculation* m_measurementTimeCalculation;
  FluxCalculation* m_fluxCalculation;
  TH1D* m_particleHistogram;
  TH1D* m_particleHistogramMirrored;
  SolarModulationFit* m_phiFit;
  int m_nBinsStart;
  int m_nBinsNew;
  TH1D* m_multiLayerEff;
  TH1D* m_trackFindingEff;
};

#endif
