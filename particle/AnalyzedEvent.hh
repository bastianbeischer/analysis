#ifndef AnalyzedEvent_hh
#define AnalyzedEvent_hh

#include "Enums.hh"
#include "ParticleInformation.hh"

#include <QVector>

class Hit;
class SimpleEvent;
class Particle;
class Settings;
class Track;

class AnalyzedEvent {
public:
  AnalyzedEvent();
  virtual ~AnalyzedEvent();

  void reset();

  bool hasTrack() const;
  bool hasGoodTrack() const;
  const Settings* settings() const;

  bool flagsMatch(Enums::EventFlags) const;
  bool flagsSet(Enums::EventFlags) const;

  const QVector<Hit*>& clusters() const {return m_clusters;}
  void setClusters(const QVector<Hit*>& clusters) {m_clusters = clusters;}

  Particle* particle() {return m_particle;}
  const Particle* particle() const {return m_particle;}
  void setParticle(Particle* particle) {m_particle = particle;}

  const SimpleEvent* simpleEvent() const {return m_simpleEvent;}
  void setSimpleEvent(SimpleEvent* event) {m_simpleEvent = event;}
private:
  QVector<Hit*> m_clusters;
  Particle* m_particle;
  SimpleEvent* m_simpleEvent;
};

#endif
