#ifndef WeightedMeanReconstruction_hh
#define WeightedMeanReconstruction_hh

#include "Reconstruction.hh"

class AnalyzedEvent;
class TMultiGraph;

class WeightedMeanReconstruction : public Reconstruction {
public:
  WeightedMeanReconstruction(Enums::LikelihoodVariables, Enums::Particles, bool additionalInformation = false);
  virtual ~WeightedMeanReconstruction();
  virtual void identify(AnalyzedEvent*);
};

#endif
