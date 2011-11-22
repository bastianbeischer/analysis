#ifndef TOFReconstruction_hh
#define TOFReconstruction_hh

#include "Reconstruction.hh"

class AnalyzedEvent;
class TMultiGraph;

class TOFReconstruction : public Reconstruction {
public:
  TOFReconstruction(Enums::LikelihoodVariables, Enums::Particles, bool additionalInformation = false);
  virtual ~TOFReconstruction();
  virtual void identify(AnalyzedEvent*);
};

#endif
