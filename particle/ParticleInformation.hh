#ifndef ParticleSelection_hh
#define ParticleSelection_hh

#include <QFlags>
#include <QMap>

class Particle;

class ParticleInformation {
public:
  ParticleInformation(const Particle* particle);
  ~ParticleInformation();
  
  enum Flag {None=0, AllTrackerLayers=1<<0, InsideMagnet=1<<1, OutsideMagnet=1<<2, HighTransverseRigidity=1<<3,
    MagnetCollision=1<<4, Albedo=1<<5, Chi2Good=1<<6, BetaGood=1<<7};

  Q_DECLARE_FLAGS(Flags, Flag);

public:
  void process();
  void reset();
  const Flags& flags() const {return m_flags;}
  const QMap<double,int>& hitsInLayers() const {return m_hitsInLayers;}
  int numberOfTrackerLayers() const {return m_hitsInLayers.size();}

private:
  void checkAllTrackerLayers();
  void checkChi2Good();
  void checkInsideMagnet();
  void checkOutsideMagnet();
  void checkHighTransverseRigidity();
  void checkMagnetCollision();
  void checkAlbedo();
  void checkBetaGood();

private:
  const Particle* m_particle;
  Flags m_flags;
  QMap<double, int> m_hitsInLayers;

};

Q_DECLARE_OPERATORS_FOR_FLAGS(ParticleInformation::Flags);

#endif /* ParticleSelection_hh */
