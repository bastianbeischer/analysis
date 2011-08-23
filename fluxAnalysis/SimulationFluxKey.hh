#ifndef SimulationFluxKey_hh
#define SimulationFluxKey_hh

#include "Particle.hh"

#include <QList>
#include <QString>
#include <QMap>
#include <QDebug>

class SimulationFluxKey {
public:
  enum Location {UndefinedLocation, GroundEsrange, Flight};
  enum Acceptance {UndefinedAcceptance, InsideMagnetAcceptance, TofAcceptance, TwoPiAcceptance};
  enum Source {UndefinedSource, TotalSource, PrimarySource, SecondarySource};

  SimulationFluxKey(const QString&, bool isAlbedo);
  SimulationFluxKey(Location, Acceptance, Source, Particle::Type, double modulationParameter, bool isAlbedo);
  ~SimulationFluxKey();

  Location location() const {return m_location;}
  Acceptance acceptance() const {return m_acceptance;}
  Source source() const {return m_source;}
  Particle::Type particle() const {return m_particle;}
  double modulationParameter() const {return m_modulationParameter;}
  bool isAlbedo() const {return m_isAlbedo;}

  QString locationName() const;
  QString acceptanceName() const;
  QString sourceName() const;
  QString particleName() const;
  QString modulationParameterName() const;

  void setModulationParameter(double);
  void setSource(Source);

  static Location location(const QString&);
  static QString locationName(Location);
  static Acceptance acceptance(const QString&);
  static QString acceptanceName(Acceptance);
  static Source source(const QString&);
  static QString sourceName(Source);
  static Particle::Type particle(const QString&);
  static QString particleName(Particle::Type);
  static double modulationParameter(const QString&);
  static QString modulationParameterName(double);

  bool operator==(const SimulationFluxKey&);

private:
  Location m_location;
  Acceptance m_acceptance;
  Source m_source;
  Particle::Type m_particle;
  double m_modulationParameter;
  bool m_isAlbedo;

  static void construct();
  static QMap<Location, QString> s_locationNames;
  static QMap<Acceptance, QString> s_acceptanceNames;
  static QMap<Source, QString> s_sourceNames;
  static QMap<Particle::Type, QString> s_particleNames;
};

bool operator<(const SimulationFluxKey& le, const SimulationFluxKey& ri);
QDebug operator<<(QDebug, const SimulationFluxKey&);

#endif
