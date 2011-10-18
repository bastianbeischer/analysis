#ifndef RigidityFluxPlot_hh
#define RigidityFluxPlot_hh

#include "PostAnalysisPlot.hh"
#include "H1DPlot.hh"
#include "Enums.hh"

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
  RigidityFluxPlot(PostAnalysisCanvas*, TH1D* particleSpectrum, double measurementTime, Enums::ChargeSign);
  ~RigidityFluxPlot();
private slots:
  void selectionChanged();
private:
  void updateBinTitles();
  void loadEfficiencies();
  TCanvas* m_canvas;
  TH1D* m_particleHistogram;
  FluxCalculation* m_fluxCalculation;
  Enums::ChargeSign m_type;
  bool m_isAlbedo;
  SolarModulationFit* m_phiFit;
  int m_nBinsStart;
  int m_nBinsNew;
//  TH1D* m_multiLayerEff;
//  TH1D* m_trackFindingEff;
};

#endif
