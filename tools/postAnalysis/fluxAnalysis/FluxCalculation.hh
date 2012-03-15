#ifndef FluxCalculation_hh
#define FluxCalculation_hh

#include "PostAnalysisPlot.hh"
#include "H1DPlot.hh"
#include "Enums.hh"

#include <QMap>
#include <QMultiMap>

class TH1D;
class FluxCalculation;

class FluxCalculation {
public:
  static FluxCalculation* instance();
  ~FluxCalculation();

  void setMeasurementTime(double);
  void setRawSpectra(const QMap<Enums::Particle, const TH1D*>&);
  void addEfficiency(Enums::Particle, TH1D*);

  double measurementTime() const;
  Enums::Particles particles() const;
  const QMap<Enums::Particle, const TH1D*>& rawSpectra() const;

  TH1D* newRawSpectrum(Enums::Particle) const;
  TH1D* newFluxSpectrum(Enums::Particle) const;
private:
  FluxCalculation();

  static FluxCalculation* s_instance;

  double m_measurementTime;
  QMap<Enums::Particle, const TH1D*> m_rigiditySpectra;
  QMultiMap<Enums::Particle, const TH1D*> m_efficiencies;
};

#endif
