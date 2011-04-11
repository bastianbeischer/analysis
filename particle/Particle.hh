#ifndef Particle_hh
#define Particle_hh

#include "Track.hh"

#include <QString>

class ParticleInformation;
class TimeOfFlight;

class Particle
{

public:
  enum Type {Unknown=0x1>>1, Proton=0x1<<0, Helium=0x1<<1, Electron=0x1<<2, Positron=0x1<<3, Muon=0x1<<4, AntiMuon=0x1<<5, PiPlus=0x1<<6, PiMinus=0x1<<7, Photon=0x1<<8};

public:
  Particle();
  Particle(Type);
  ~Particle();

  void setType(const Type);
  void setTrackType(const Track::Type);

  Type type() const {return m_type;}
  int pdgId() const {return m_pdgId;}
  Track* track() const {return m_track;}
  QString name() const {return m_name;}
  TimeOfFlight* timeOfFlight() const {return m_tof;}
  ParticleInformation* information() const {return m_information;}

  double mass() const {return m_mass;}
  double charge() const {return m_charge;}
  double transverseMomentum() const {return m_charge*m_track->transverseRigidity();}
  double momentum() const {return m_charge*m_track->rigidity();}
  double beta() const;

private:
  void init();

private:
  Type m_type;
  double m_mass;
  int m_charge;
  int m_pdgId;
  QString m_name;

  Track* m_track;
  TimeOfFlight* m_tof;
  ParticleInformation* m_information;

};

#endif /* Particle_hh */
