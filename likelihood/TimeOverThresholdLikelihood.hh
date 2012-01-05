#ifndef TimeOverThresholdLikelihood_hh
#define TimeOverThresholdLikelihood_hh

#include "Likelihood.hh"

class TimeOverThresholdLikelihood : public Likelihood {
public:
  TimeOverThresholdLikelihood(Enums::Particles = Enums::NoParticle);
  ~TimeOverThresholdLikelihood();
  virtual double eval(const AnalyzedEvent*, const Hypothesis&, bool* goodInterpolation = 0) const;
  virtual double eval(double timeOverThreshold, const Hypothesis&, bool* goodInterpolation = 0) const;
  virtual double eval(double timeOverThreshold, const Hypothesis&, const PDFParameters&) const;
};

#endif
