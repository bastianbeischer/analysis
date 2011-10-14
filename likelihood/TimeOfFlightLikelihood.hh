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
  virtual double eval(double measuredBeta, Enums::Particle, double reconstructedMomentum) const;
private:
  static const double s_cutoffMomentum;
};

#endif
