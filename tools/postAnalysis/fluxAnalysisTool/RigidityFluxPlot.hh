#ifndef RigidityFluxPlot_hh
#define RigidityFluxPlot_hh

#include "PostAnalysisPlot.hh"
#include "H1DPlot.hh"
#include <QMap>

class TH1D;
class FluxCalculation;
class SolarModulationFit;
class PostAnalysisCanvas;

class RigidityFluxPlot : public PostAnalysisPlot, public H1DPlot {
public:
  enum Type {
    Positive,
    Negative,
  };
  RigidityFluxPlot(PostAnalysisCanvas* canvas, double measurementTime, Type type);
  virtual ~RigidityFluxPlot();

private:
  TH1D* m_particleHistogram;
  FluxCalculation* m_fluxCalculation;
  Type m_type;
  bool m_isAlbedo;
  SolarModulationFit* m_phiFit;
  int m_nBinsStart;
  int m_nBinsNew;

  void updateBinTitles();
  void loadEfficiencies();
  static bool s_efficienciesLoaded;

  static TH1D* s_multiLayerEff;
  static TH1D* s_trackFindingEff;
  QMap<Type, QString> m_typeNames;
};

#endif
