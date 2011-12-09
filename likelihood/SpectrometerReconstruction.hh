#ifndef SpectrometerReconstruction_hh
#define SpectrometerReconstruction_hh

#include "Reconstruction.hh"
#include "Hypothesis.hh"

class AnalyzedEvent;
class TMultiGraph;

class SpectrometerReconstruction : public Reconstruction {
public:
  SpectrometerReconstruction(Enums::LikelihoodVariables, Enums::Particles, bool additionalInformation = false);
  virtual void identify(AnalyzedEvent*);
};

#endif
