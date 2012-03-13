#ifndef FluxCalculation_hh
#define FluxCalculation_hh

#include "PostAnalysisPlot.hh"
#include "H1DPlot.hh"
#include "Enums.hh"

#include <QMap>

class TH1D;
class FluxCalculation;

class FluxCalculation {
public:
  static FluxCalculation* instance();
  ~FluxCalculation();

  void setRawSpectra(QMap<Enums::Particle, const TH1D*>); 
  void setMeasurementTime(double);

  Enums::Particles particles() const;
  const QMap<Enums::Particle, const TH1D*>& rawSpectra() const;
  double measurementTime() const;

  TH1D* newRawSpectrum(Enums::Particle) const;
  TH1D* newSummedRawSpectrum(Enums::Particles) const;
private:
  FluxCalculation();

  static FluxCalculation* s_instance;

  QMap<Enums::Particle, const TH1D*> m_rigiditySpectra;
  double m_measurementTime;
};

#endif
