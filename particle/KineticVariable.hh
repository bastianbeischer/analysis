#ifndef KineticVariable_hh
#define KineticVariable_hh

#include "Enums.hh"
#include "ParticleProperties.hh"

class KineticVariable {
public:
  KineticVariable(Enums::Particle);
  KineticVariable(Enums::Particle, Enums::KineticVariable, double value);

  const ParticleProperties& properties() const;
  Enums::Particle particle() const;
  int charge() const;
  double mass() const;
  double totalEnergy() const;
  double kineticEnergy() const;
  double momentum() const;
  double rigidity() const;
  double curvature() const;
  double beta() const;
  double inverseBeta() const;

  void set(Enums::KineticVariable, double value);
  void setTotalEnergy(double);
  void setKineticEnergy(double);
  void setMomentum(double);
  void setRigidity(double);
  void setCurvature(double);
  void setBeta(double);
  void setInverseBeta(double);
  
private:
  ParticleProperties m_properties;
  double m_kineticEnergy;
};

#endif
