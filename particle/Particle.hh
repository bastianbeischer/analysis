#ifndef Particle_hh
#define Particle_hh

#include "Track.hh"

class ParticleInformation;

class Particle
{

public:
  enum Type {None=-1, Proton, Electron, Positron, Muon, AntiMuon, Helium, Pion};

public:
  Particle();
  Particle(Type);
  ~Particle();

  void setType(Type);
  void setTrackType(Track::Type);

  Track* track() const {return m_track;}
  ParticleInformation* information() const {return m_information;}

  double mass() const {return m_mass;}
  double charge() const {return m_charge;}
  double momentum() const {return m_charge*m_track->rigidity();}

private:
  Type m_type;

  Track* m_track;
  ParticleInformation* m_information;

  double m_mass;
  int m_charge;
  
};

#endif /* Particle_hh */
