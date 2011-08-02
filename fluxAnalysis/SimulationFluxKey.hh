#ifndef SimulationFluxKey_hh
#define SimulationFluxKey_hh

#include "Particle.hh"

#include <QList>
#include <QString>
#include <QMap>

class SimulationFluxKey {

public:
  enum Source {
    Total,
    Primary,
    Secondary
  };

  SimulationFluxKey(double phi, Particle::Type type, bool isAlbedo, Source source);
	~SimulationFluxKey();

  double phi() const {return m_phi;}
  Particle::Type type() const {return m_type;}
  bool isAlbedo() const {return m_isAlbedo;}
  Source source() const {return m_source;}
  QString sourceName();
  QString internalName();
  QString name();

  void setPhi(double phi);
  void setSource(Source source);

  static QList<Source> allSources();
  static QString sourceName(Source source);
  static Source source(QString sourceName);

  static QList<Particle::Type> allParticles();

  bool operator==(SimulationFluxKey);

private:
  double m_phi;
  Particle::Type m_type;
  bool m_isAlbedo;
  Source m_source;

  static bool s_constructed;
  static void construct();
  static QMap<Particle::Type, QString> s_particleNames;
  static QMap<Source, QString> s_sourceNames;

};

inline bool operator<(const SimulationFluxKey& le, const SimulationFluxKey& ri)
{
  if (le.phi() != ri.phi()) {
    return le.phi() < ri.phi();
  } else {
    if (le.source() != ri.source()) {
      return le.source() < ri.source();
    } else {
      if (le.type() != ri.type()) {
        return le.type() < ri.type();
      } else {
        return le.isAlbedo() < ri.isAlbedo();
      }
    }
  }
}

#endif
