#ifndef RigidityFluxPlot_hh
#define RigidityFluxPlot_hh

#include "PostAnalysisCanvas.hh"
#include "PostAnalysisPlot.hh"
#include "H1DPlot.hh"
#include "FluxCalculation.hh"
#include "SolarModulationFit.hh"

#include <TH1D.h>

#include <QMap>

class RigidityFluxPlot : public PostAnalysisPlot, public H1DPlot {
public:
  enum Type {
    All,
    Positive,
    Negative,
    AlbedoPositive,
    AlbedoNegative,
  };
  RigidityFluxPlot(PostAnalysisCanvas* canvas, double measurementTime);
  virtual ~RigidityFluxPlot();
  Type type(const QString&);

private:
  TH1D* m_particleHistogram;
  FluxCalculation* m_fluxCalculation;
  SolarModulationFit* m_phiFit;
  Type m_type;

  void updateBinTitles();
  void loadEfficiencies();
  static bool s_efficienciesLoaded;

  static QMap<Type, TH1D*> s_multiLayerEff;
  static QMap<Type, TH1D*> s_trackFindingEff;
  QMap<Type, QString> m_typeNames;
};

#endif
