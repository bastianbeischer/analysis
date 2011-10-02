#ifndef Particle_hh
#define Particle_hh

#include "Track.hh"

#include <TColor.h>

#include <QString>

class ParticleProperties;
class ParticleInformation;
class TimeOfFlight;
class TRDReconstruction;

class Particle
{

public:
  enum Type {START=1<<0, Unknown=START, Proton=1<<1, AntiProton=1<<2, Helium=1<<3, Electron=1<<4, Positron=1<<5,
             Muon=1<<6, AntiMuon=1<<7, PiPlus=1<<8, PiMinus=1<<9, Photon=1<<10, Pi0=1<<11, Higgs=1<<12, END=Higgs};
  Q_DECLARE_FLAGS(Types, Type);

public:
  Particle();
  Particle(const Type&);
  Particle(const int&);
  ~Particle();

  void setType(const Type&);
  void setPdgId(const int&);
  void setTrackType(const Enums::TrackType&);

  const ParticleProperties* properties() const {return m_properties;}
  Track* track() const {return m_track;}
  TimeOfFlight* timeOfFlight() const {return m_tof;}
  TRDReconstruction* trdReconstruction() const {return m_trd;}
  ParticleInformation* information() const {return m_information;}

  double transverseMomentum() const;
  double momentum() const;
  double beta() const;

  // loopthrough functions to properties
  Type type() const;
  int pdgId() const;
  QString name() const;
  double mass() const;
  int charge() const;
  Color_t color() const;

  void reset();

private:
  void init();

private:
  const ParticleProperties* m_properties;
  Track* m_track;
  TimeOfFlight* m_tof;
  TRDReconstruction* m_trd;
  ParticleInformation* m_information;

};

Q_DECLARE_OPERATORS_FOR_FLAGS(Particle::Types);

#endif /* Particle_hh */
