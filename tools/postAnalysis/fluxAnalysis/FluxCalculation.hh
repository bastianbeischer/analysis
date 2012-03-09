#ifndef FluxCalculation_hh
#define FluxCalculation_hh

#include "PostAnalysisPlot.hh"
#include "H1DPlot.hh"
#include "Enums.hh"

#include <QObject>
#include <QMap>

class TH1D;
class FluxCalculation;

class FluxCalculation {
public:
  FluxCalculation(QMap<Enums::Particle, const TH1D*>);
  ~FluxCalculation();
  TH1D* newRawSpectrum(Enums::Particle);
  TH1D* newSummedRawSpectrum(Enums::Particles);
private:
  QMap<Enums::Particle, const TH1D*> m_rigiditySpectra;
};

#endif
