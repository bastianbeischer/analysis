#ifndef SmearedRigidityComparissonPlot_hh
#define SmearedRigidityComparissonPlot_hh

#include "PostAnalysisPlot.hh"
#include "H1DPlot.hh"

#include <QMap>
#include <QString>

class TH1D;

class SmearedRigidityComparissonPlot : public PostAnalysisPlot, public H1DPlot {
public:
  enum Type {Positive, Negative};
  SmearedRigidityComparissonPlot(Type type, TH1D* generatedSpectrum, TH1D* reconstructedSpectrum, TH1D* unfoldedSpectrum = 0);
  ~SmearedRigidityComparissonPlot();
private:
  void scaleToBinWidth(TH1D* histogram);
  Type m_type;
  TH1D* m_generatedSpectrum;
  TH1D* m_reconstructedSpectrum;
  TH1D* m_unfoldedSpectrum;
  QMap<Type, QString> m_typeNames;
};

#endif
