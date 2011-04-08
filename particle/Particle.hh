#ifndef Particle_hh
#define Particle_hh

#include "Track.hh"

class ParticleInformation;
class TimeOfFlight;

class Particle
{

public:
  enum Type {Unknown=-1, START=0, Proton=START, Electron, Positron, Muon, AntiMuon, Helium, END};

public:
  Particle();
  Particle(Type);
  ~Particle();

  void setType(const Type);
  void setTrackType(const Track::Type);

  Type type() const {return m_type;}
  Track* track() const {return m_track;}
  TimeOfFlight* timeOfFlight() const {return m_tof;}
  ParticleInformation* information() const {return m_information;}

  double mass() const {return m_mass;}
  double charge() const {return m_charge;}
  double transverseMomentum() const {return m_charge*m_track->transverseRigidity();}
  double momentum() const {return m_charge*m_track->rigidity();}
  double beta() const;

private:
  Type m_type;
  Track* m_track;
  TimeOfFlight* m_tof;
  ParticleInformation* m_information;

  double m_mass;
  int m_charge;
  
};

#endif /* Particle_hh */
