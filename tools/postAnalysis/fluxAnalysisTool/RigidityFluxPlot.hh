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
class TGraph;
class TGraphAsymmErrors;
class SimulationFluxWidget;
class BessFluxWidget;

class RigidityFluxPlot : public QObject, public PostAnalysisPlot, public H1DPlot {
Q_OBJECT
public:
  RigidityFluxPlot(PostAnalysisCanvas*, TH1D* particleSpectrum, double measurementTime, Enums::ChargeSign, TH1D* particleSpectrumLow = 0, TH1D* particleSpectrumUp = 0);
  ~RigidityFluxPlot();
  virtual void draw(TCanvas* canvas);
private slots:
  void selectionChanged();
private:
  void updateBinTitles();
  TCanvas* m_canvas;
  TH1D* m_particleHistogram;
  TH1D* m_particleHistogramLow;
  TH1D* m_particleHistogramUp;
  FluxCalculation* m_fluxCalculation;
  Enums::ChargeSign m_type;
  bool m_isAlbedo;
  SolarModulationFit* m_phiFit;
  int m_nBinsStart;
  int m_nBinsNew;
  TGraphAsymmErrors* m_sysbelt;
  SimulationFluxWidget* m_simulationWidget;
  BessFluxWidget* m_bessWidget;
};

#endif
