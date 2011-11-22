#ifndef TrackerMomentumLikelihood_hh
#define TrackerMomentumLikelihood_hh

#include "Likelihood.hh"
#include "Enums.hh"

class TrackerMomentumLikelihood : public Likelihood {
public:
  TrackerMomentumLikelihood(Enums::Particles = Enums::NoParticle);
  ~TrackerMomentumLikelihood();

  virtual double eval(const AnalyzedEvent*, const Hypothesis&, bool* goodInterpolation = 0) const;
  virtual double eval(double curvature, const Hypothesis&, bool* goodInterpolation = 0) const;
};

#endif
