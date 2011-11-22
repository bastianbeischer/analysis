#ifndef Hypothesis_hh
#define Hypothesis_hh

#include "Enums.hh"
#include "ParticleProperties.hh"

class Hypothesis {
public:
  Hypothesis(Enums::Particle, double curvature);
  ~Hypothesis();
  
  double propability() const;
  void setProbability(double);

  Enums::Particle particle() const;
  int charge() const;
  double mass() const;
  double curvature() const;
  double rigidity() const;
  double beta() const;
  double inverseBeta() const;

  void dump() const;
private:
  ParticleProperties m_properties;
  double m_curvature;
  double m_probability;
};

#endif
