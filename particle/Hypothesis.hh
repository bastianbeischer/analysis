#ifndef Hypothesis_hh
#define Hypothesis_hh

#include "Enums.hh"
#include "ParticleProperties.hh"

class Hypothesis {
public:
  Hypothesis(Enums::Particle, double curvature);
  ~Hypothesis();

  double likelihood() const;
  int numberOfLikelihoods() const;
  void setLikelihood(int, double);

  Enums::Particle particle() const;
  int charge() const;
  double mass() const;
  double curvature() const;
  double rigidity() const;
  double absoluteRigidity() const;
  double beta() const;
  double inverseBeta() const;

  bool operator<(const Hypothesis&) const;
private:
  ParticleProperties m_properties;
  double m_curvature;
  int m_nLikelihoods;
  double m_likelihood;
};

QDebug operator<<(QDebug, const Hypothesis&);

#endif
