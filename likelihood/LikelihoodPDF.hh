#ifndef LikelihoodPDF_hh
#define LikelihoodPDF_hh

#include "Enums.hh"

#include <TF1.h>

class Likelihood;

class LikelihoodPDF : public TF1 {
public:
  LikelihoodPDF(const Likelihood*, Enums::Particle, double momentum);
  ~LikelihoodPDF();
  Enums::Particle particle() const;
  double momentum() const;
  double integral();
private:
  double rootFunctionPointer(double*, double*) const;

  const Likelihood* m_likelihood;
  Enums::Particle m_particle;
  double m_momentum;
};

#endif
