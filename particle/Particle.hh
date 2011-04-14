#ifndef Particle_hh
#define Particle_hh

#include "Track.hh"

#include <QString>

class ParticleProperties;
class ParticleInformation;
class TimeOfFlight;

class Particle
{

public:
  enum Type {START=0x1<<0, Unknown=START, Proton=0x1<<1, Helium=0x1<<2, Electron=0x1<<3, Positron=0x1<<4,
             Muon=0x1<<5, AntiMuon=0x1<<6, PiPlus=0x1<<7, PiMinus=0x1<<8, Photon=0x1<<9, END=Photon};

public:
  Particle();
  Particle(const Type&);
  Particle(const int&);
  ~Particle();

  void setType(const Type&);
  void setPdgId(const int&);
  void setTrackType(const Track::Type&);

  const ParticleProperties* properties() const {return m_properties;}
  Track* track() const {return m_track;}
  TimeOfFlight* timeOfFlight() const {return m_tof;}
  ParticleInformation* information() const {return m_information;}

  double transverseMomentum() const;
  double momentum() const;
  double beta() const;

  // loopthrough functions to properties
  Type type() const;
  int pdgId() const;
  QString name() const;
  double mass() const;
  double charge() const;

private:
  void init();

private:
  const ParticleProperties* m_properties;
  Track* m_track;
  TimeOfFlight* m_tof;
  ParticleInformation* m_information;

};

#endif /* Particle_hh */
