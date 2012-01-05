#ifndef CherenkovLikelihood_hh
#define CherenkovLikelihood_hh

#include "Likelihood.hh"
#include "Enums.hh"

class CherenkovLikelihood : public Likelihood {
public:
  CherenkovLikelihood(Enums::Particles);
  ~CherenkovLikelihood();
  virtual double eval(const AnalyzedEvent*, const Hypothesis&, bool* goodInterpolation = 0) const;
  virtual double eval(double, const Hypothesis&, bool* goodInterpolation = 0) const;
  virtual double eval(double, const Hypothesis&, const PDFParameters&) const;
private:
  int m_numberOfParticles;
};

#endif
