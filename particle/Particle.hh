#ifndef Particle_hh
#define Particle_hh

#include "Track.hh"
#include "Enums.hh"

#include <TColor.h>

#include <QString>

class ParticleProperties;
class ParticleInformation;
class TimeOfFlight;
class TRDReconstruction;
class Hypothesis;

class Particle {
public:
  Particle();
  Particle(const Enums::Particle&);
  Particle(const int&);
  ~Particle();

  void setType(const Enums::Particle&);
  void setPdgId(const int&);
  void setTrackType(const Enums::TrackType&);

  const ParticleProperties* properties() const {return m_properties;}
  Track* track() const {return m_track;}
  TimeOfFlight* timeOfFlight() const {return m_tof;}
  TRDReconstruction* trdReconstruction() const {return m_trd;}
  ParticleInformation* information() const {return m_information;}

  void setReconstructionMethod(Enums::ReconstructionMethod);
  Enums::ReconstructionMethod reconstructionMethod() const;

  void addHypothesis(Enums::ReconstructionMethod, Hypothesis*);
  const QMap<Enums::ReconstructionMethod, Hypothesis*>& hypotheses() const;
  const Hypothesis* hypothesis();
  const Hypothesis* hypothesis(Enums::Particle) const;
  const Hypothesis* hypothesis(Enums::Particle, Enums::ReconstructionMethod) const;

  double transverseMomentum() const; // To be removed: KineticVariable that
  double momentum() const;           // contains Likelihood analyzed data
  double beta() const;               // should be used!

  // loopthrough functions to properties
  Enums::Particle type() const;
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
  Enums::ReconstructionMethod m_method;
  QMap<Enums::ReconstructionMethod, Hypothesis*> m_hypotheses;
};

#endif /* Particle_hh */
