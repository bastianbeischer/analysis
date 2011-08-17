#ifndef RigidityFluxPlot_hh
#define RigidityFluxPlot_hh

#include "PostAnalysisPlot.hh"
#include "H1DPlot.hh"
#include <QObject>
#include <QMap>

class TH1D;
class TCanvas;
class FluxCalculation;
class SolarModulationFit;
class PostAnalysisCanvas;

class RigidityFluxPlot : public QObject, public PostAnalysisPlot, public H1DPlot {
Q_OBJECT
public:
  enum Type {Positive, Negative};
  RigidityFluxPlot(PostAnalysisCanvas*, TH1D* particleSpectrum, double measurementTime, Type type);
  ~RigidityFluxPlot();
private slots:
  void selectionChanged();
private:
  TCanvas* m_canvas;
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
