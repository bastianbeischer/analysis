#ifndef RigidityFlux_hh
#define RigidityFlux_hh

#include "H1DPlot.hh"
#include "AnalysisPlot.hh"
#include "FluxCalculation.hh"
#include "MeasurementTimeCalculation.hh"
#include "EfficiencyCorrectionSettings.hh"
#include "SolarModulationFit.hh"
#include "Plotter.hh"
#include "Enums.hh"

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
  RigidityFlux(Enums::ChargeSign, const QDateTime& first, const QDateTime& last, TH1D* particleHistogram);
  ~RigidityFlux();

  virtual void processEvent(const AnalyzedEvent*);
  virtual void update();
private slots:
  void selectionChanged();

private:
  void loadEfficiencies();
  void efficiencyCorrection();
  void updateBinTitles();
  Enums::ChargeSign m_type;
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
