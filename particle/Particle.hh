#ifndef Particle_hh
#define Particle_hh

#include "Track.hh"

class Particle
{

public:
  enum Type {None=-1, Proton, Electron, Positron, Muon, AntiMuon, Helium, Pion};

public:
  Particle();
  Particle(Type);
  ~Particle();

  void setType(Type type);
  void setTrack(const Track* track) {m_track = track;}

  const Track* track() const {return m_track;}
  double mass() const {return m_mass;}
  double charge() const {return m_charge;}
  double momentum() const {return m_charge*m_track->rigidity();}

private:
  Type m_type;
  const Track* m_track;
  double m_mass;
  int m_charge;
  
};

#endif /* Particle_hh */
