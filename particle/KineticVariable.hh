#ifndef KineticVariable_hh
#define KineticVariable_hh

#include "Enums.hh"

#include <cmath>

class KineticVariable {
public:
  enum Type {TotalEnergy, KineticEnergy, Momentum, Rigidity, Curvature, Beta};

  KineticVariable(Enums::Particle particle, double kineticEnergy = 0);

  int charge() const;
  double mass() const;
  double totalEnergy() const;
  double kineticEnergy() const;
  double momentum() const;
  double rigidity() const;
  double curvature() const;
  double beta() const;

  void set(Type type, double value);
  void setTotalEnergy(double value);
  void setKineticEnergy(double value);
  void setMomentum(double value);
  void setRigidity(double value);
  void setCurvature(double value);
  void setBeta(double value);
  
private:
  int m_charge;
  double m_mass;
  double m_kineticEnergy;
};

#endif
