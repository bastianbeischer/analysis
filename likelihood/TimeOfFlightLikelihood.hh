#ifndef TimeOfFlightLikelihood_hh
#define TimeOfFlightLikelihood_hh

#include "Likelihood.hh"
#include "Enums.hh"

class TimeOfFlightLikelihood : public Likelihood {
public:
  TimeOfFlightLikelihood(Enums::Particles = Enums::NoParticle);
  ~TimeOfFlightLikelihood();
  virtual double eval(const AnalyzedEvent*, const Hypothesis&, bool* goodInterpolation = 0) const;
  virtual double eval(double inverseBeta, const Hypothesis&, bool* goodInterpolation = 0) const;
  virtual double eval(double inverseBeta, const Hypothesis&, const PDFParameters&) const;
};

#endif
