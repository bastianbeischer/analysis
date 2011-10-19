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
  virtual double eval(Particle*, const KineticVariable&, bool* goodInterpolation = 0) const;
  virtual double eval(double curvature, const KineticVariable&, bool* goodInterpolation = 0) const;
};

#endif
