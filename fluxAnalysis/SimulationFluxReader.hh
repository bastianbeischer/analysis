#ifndef SimulationFluxReader_hh
#define SimulationFluxReader_hh

#include "Particle.hh"
#include "SimulationFluxKey.hh"

#include <QMap>
#include <QVector>
#include <QString>

class TH1D;

class SimulationFluxReader {
public:
  static SimulationFluxReader* instance();
	~SimulationFluxReader();
  const QVector<SimulationFluxKey::Location>& locations() const;
  const QVector<SimulationFluxKey::Acceptance>& acceptances() const;
  const QVector<SimulationFluxKey::Source>& sources() const;
  const QVector<Particle::Type>& particles() const;
  const QVector<double>& modulationParameters() const;
  TH1D* spectrum(const SimulationFluxKey&) const;
  TH1D* spectrum(const QList<SimulationFluxKey>&) const;
private:
  SimulationFluxReader();
  void readKeys(const QString& fileName);

  static SimulationFluxReader* s_instance;
  QVector<SimulationFluxKey::Location> m_locations;
  QVector<SimulationFluxKey::Acceptance> m_acceptances;
  QVector<SimulationFluxKey::Source> m_sources;
  QVector<Particle::Type> m_particles;
  QVector<double> m_modulationParameters;
  QMap<SimulationFluxKey, TH1D*> m_fluxes;
};

#endif
