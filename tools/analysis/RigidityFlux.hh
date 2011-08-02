#ifndef RigidityFlux_hh
#define RigidityFlux_hh

#include "AnalysisPlot.hh"
#include "H1DPlot.hh"
#include "FluxCalculation.hh"
#include "MeasurementTimeCalculation.hh"
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

class RigidityFlux : public AnalysisPlot, public H1DPlot {

public:
  enum Type {
    All,
    Positive,
    Negative,
    AlbedoPositive,
    AlbedoNegative,
  };

  RigidityFlux(Type type, const QDateTime& first, const QDateTime& last, Plotter* plotter, TH1D* particleHistogram);
  ~RigidityFlux();

  virtual void processEvent(const QVector<Hit*>&, const Particle* const = 0, const SimpleEvent* const = 0);
  virtual void finalize();
  virtual void update();

private:
  Type m_type;
  MeasurementTimeCalculation* m_measurementTimeCalculation;
  FluxCalculation* m_fluxCalculation;
  Plotter* m_plotter;
  TH1D* m_particleHistogram;
  void updateBinTitles();
//  double maxFlux();

  void loadEfficiencies();
  static bool s_efficienciesLoaded;
  void efficiencyCorrection();

  static QMap<Type, TH1D*> s_multiLayerEff;
  static QMap<Type, TH1D*> s_trackFindingEff;
};

#endif
