#ifndef Hypothesis_hh
#define Hypothesis_hh

#include "Enums.hh"
#include "ParticleProperties.hh"

class Hypothesis {
public:
  Hypothesis(Enums::Particle, double curvature);
  ~Hypothesis();
  
  double logLikelihood() const;
  void setLogLikelihood(double);

  Enums::Particle particle() const;
  int charge() const;
  double mass() const;
  double curvature() const;
  double rigidity() const;
  double absoluteRigidity() const;
  double beta() const;
  double inverseBeta() const;
private:
  ParticleProperties m_properties;
  double m_curvature;
  double m_logLikelihood;
};

QDebug operator<<(QDebug, const Hypothesis&);

#endif
