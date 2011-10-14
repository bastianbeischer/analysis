#ifndef LikelihoodRatio_hh
#define LikelihoodRatio_hh

#include "Enums.hh"

#include <TF1.h>

class Likelihood;

class LikelihoodRatio : public TF1 {
public:
  LikelihoodRatio(const Likelihood*, Enums::Particle, double momentum, double min, double max);
  ~LikelihoodRatio();
  Enums::Particle particle() const;
  double momentum() const;
private:
  double rootFunctionPointer(double*, double*) const;

  const Likelihood* m_likelihood;
  Enums::Particle m_particle;
  double m_momentum;
};

#endif
