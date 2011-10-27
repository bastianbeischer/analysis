#ifndef TimeOfFlightLikelihood_hh
#define TimeOfFlightLikelihood_hh

#include "Likelihood.hh"
#include "Enums.hh"

class TimeOfFlightLikelihood : public Likelihood {
public:
  TimeOfFlightLikelihood();
  ~TimeOfFlightLikelihood();

  virtual double eval(const Particle*, const KineticVariable& hypothesis, bool* goodInterpolation = 0) const;
  virtual double eval(double inverseBeta, const KineticVariable& hypothesis, bool* goodInterpolation = 0) const;
};

#endif
