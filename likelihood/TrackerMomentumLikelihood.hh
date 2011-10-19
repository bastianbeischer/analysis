#ifndef TrackerMomentumLikelihood_hh
#define TrackerMomentumLikelihood_hh

#include "Likelihood.hh"
#include "Enums.hh"

class TrackerMomentumLikelihood : public Likelihood {
public:
  TrackerMomentumLikelihood();
  ~TrackerMomentumLikelihood();

  virtual double min() const;
  virtual double max() const;
  virtual int numberOfParameters() const;
  virtual double eval(double measuredBeta, Enums::Particle, double realMomentum, bool* goodInterpolation = 0) const;
  virtual ParameterVector defaultParameters() const;
};

#endif
