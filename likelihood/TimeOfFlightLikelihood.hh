#ifndef TimeOfFlightLikelihood_hh
#define TimeOfFlightLikelihood_hh

#include "Likelihood.hh"
#include "Enums.hh"

class TimeOfFlightLikelihood : public Likelihood {
public:
  TimeOfFlightLikelihood();
  ~TimeOfFlightLikelihood();
  virtual double min() const;
  virtual double max() const;
  virtual int numberOfParameters() const;
  virtual double eval(Particle*, const KineticVariable& hypothesis, bool* goodInterpolation = 0) const;
  virtual double eval(double inverseBeta, const KineticVariable& hypothesis, bool* goodInterpolation = 0) const;
};

#endif
