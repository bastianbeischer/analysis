#ifndef CherenkovLikelihood_hh
#define CherenkovLikelihood_hh

#include "Likelihood.hh"
#include "Enums.hh"

class CherenkovLikelihood : public Likelihood {
public:
  CherenkovLikelihood(Enums::Particles);
  ~CherenkovLikelihood();
  virtual double eval(const AnalyzedEvent*, const Hypothesis& hypothesis, bool* goodInterpolation = 0) const;
  virtual double eval(double inverseBeta, const Hypothesis& hypothesis, bool* goodInterpolation = 0) const;
private:
  int m_numberOfParticles;
};

#endif
