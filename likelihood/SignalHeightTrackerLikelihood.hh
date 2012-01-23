#ifndef SignalHeightTrackerLikelihood_hh
#define SignalHeightTrackerLikelihood_hh

#include "Likelihood.hh"

class SignalHeightTrackerLikelihood : public Likelihood {
public:
  SignalHeightTrackerLikelihood(Enums::Particles = Enums::NoParticle);
  ~SignalHeightTrackerLikelihood();
  virtual double eval(const AnalyzedEvent*, const Hypothesis&, bool* goodInterpolation = 0) const;
  virtual double eval(double signalHeight, const Hypothesis&, bool* goodInterpolation = 0) const;
  virtual double eval(double signalHeight, const Hypothesis&, const PDFParameters&) const;
};

#endif
