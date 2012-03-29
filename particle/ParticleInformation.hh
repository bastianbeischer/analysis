#ifndef ParticleSelection_hh
#define ParticleSelection_hh

#include "Enums.hh"

#include <QFlags>
#include <QMap>

class Particle;
class Track;

class ParticleInformation {
public:
  ParticleInformation(const Particle*);
  ~ParticleInformation();

public:
  void process();
  void reset();
  const Enums::EventFlags& eventFlags() const {return m_flags;}
  const QMap<double,int>& hitsInLayers() const {return m_hitsInLayers;}
  int numberOfTrackerLayers() const {return m_hitsInLayers.size();}

private:
  void checkAllTrackerLayers();
  void checkChi2Good();
  void checkMagnet();
  void checkHighTransverseRigidity();
  void checkAlbedo();
  void checkBetaGood();
  void checkTrackGood();

private:
  const Particle* m_particle;
  Enums::EventFlags m_flags;
  QMap<double, int> m_hitsInLayers;

};

#endif /* ParticleSelection_hh */
