#ifndef SimulationFluxReader_hh
#define SimulationFluxReader_hh

#include "Enums.hh"
#include "SimulationFluxKey.hh"

#include <QMap>
#include <QVector>
#include <QString>

class TH1D;

class SimulationFluxReader {
public:
  static SimulationFluxReader* instance();
  ~SimulationFluxReader();
  QVector<SimulationFluxKey::Location> locations() const;
  QVector<SimulationFluxKey::Acceptance> acceptances(SimulationFluxKey::Location) const;
  QVector<SimulationFluxKey::Source> sources(SimulationFluxKey::Location) const;
  QVector<Enums::Particle> particles(SimulationFluxKey::Location) const;
  QVector<double> modulationParameters(SimulationFluxKey::Location) const;
  TH1D* spectrum(const SimulationFluxKey&) const;
  TH1D* spectrum(const QVector<SimulationFluxKey>&) const;
private:
  SimulationFluxReader();
  void readKeys(const QString& fileName);

  static SimulationFluxReader* s_instance;
  QMap<SimulationFluxKey, TH1D*> m_fluxes;
};

#endif
