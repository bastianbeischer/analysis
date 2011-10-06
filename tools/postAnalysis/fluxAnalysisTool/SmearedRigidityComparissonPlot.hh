#ifndef SmearedRigidityComparissonPlot_hh
#define SmearedRigidityComparissonPlot_hh

#include "PostAnalysisPlot.hh"
#include "H1DPlot.hh"
#include "Enums.hh"

#include <QMap>
#include <QString>

class TH1D;

class SmearedRigidityComparissonPlot : public PostAnalysisPlot, public H1DPlot {
public:
  SmearedRigidityComparissonPlot(Enums::ChargeSign, TH1D* generatedSpectrum, TH1D* reconstructedSpectrum, TH1D* unfoldedSpectrum = 0);
  ~SmearedRigidityComparissonPlot();
private:
  void scaleToBinWidth(TH1D* histogram);
  Enums::ChargeSign m_type;
  TH1D* m_generatedSpectrum;
  TH1D* m_reconstructedSpectrum;
  TH1D* m_unfoldedSpectrum;
};

#endif
