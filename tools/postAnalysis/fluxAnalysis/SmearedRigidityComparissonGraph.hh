#ifndef SmearedRigidityComparissonGraph_hh
#define SmearedRigidityComparissonGraph_hh

#include "PostAnalysisPlot.hh"
#include "H1DPlot.hh"
#include "Enums.hh"

#include <QMap>
#include <QString>

class TCanvas;
class TH1D;
class TGraph;
class TGraphAsymmErrors;

class SmearedRigidityComparissonGraph : public PostAnalysisPlot, public H1DPlot {
public:
  enum Type {Positive, Negative};
  SmearedRigidityComparissonGraph(Enums::ChargeSign type, TH1D* generatedSpectrum, TH1D* reconstructedSpectrum, TH1D* reconstructedSpectrumLow, TH1D* reconstructedSpectrumUp);
  ~SmearedRigidityComparissonGraph();
  virtual void draw(TCanvas* canvas);
private:
  Enums::ChargeSign m_type;
  TH1D* m_generatedSpectrum;
  TH1D* m_reconstructedSpectrum;
  TH1D* m_reconstructedSpectrumLow;
  TH1D* m_reconstructedSpectrumUp;
  TGraphAsymmErrors* m_sysbelt;
};

#endif
