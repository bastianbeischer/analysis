#ifndef SimulationFluxParticle_hh
#define SimulationFluxParticle_hh

#include "Particle.hh"

#include <QList>
#include <QString>
#include <QMap>

class SimulationFluxParticle {

public:
  enum Source {
    Total,
    Primary,
    Secondary
  };

  SimulationFluxParticle(Particle::Type type, bool isAlbedo, Source source);
	~SimulationFluxParticle();

  Particle::Type type() const {return m_type;}
  bool isAlbedo() const {return m_isAlbedo;}
  Source source() const {return m_source;}
  QString internalName() const;

  static QList<Source> allSources();
  static QString sourceName(Source source);
  static Source source(QString sourceName);
  QString sourceName() const;

  static QList<Particle::Type> allParticles();
  QString name() const;
  bool operator==(SimulationFluxParticle);

private:
  Particle::Type m_type;
  bool m_isAlbedo;
  Source m_source;

  static bool s_constructed;
  static void construct();
  static QMap<Particle::Type, QString> s_particleNames;
  static QMap<Source, QString> s_sourceNames;

};


#endif
