#ifndef TOTMomentumCorrelation_hh
#define TOTMomentumCorrelation_hh

#include "H2DPlot.hh"
#include "TOTLayer.hh"

#include <QString>

class Hit;
class Particle;
class SimpleEvent;

class TOTMomentumCorrelation : public H2DPlot, public TOTLayer {
public:
  TOTMomentumCorrelation();
  TOTMomentumCorrelation(TOTLayer::Layer layer);
  ~TOTMomentumCorrelation();
  TOTMomentumCorrelation* create(TOTLayer::Layer layer) const;
  virtual void processEvent(const AnalyzedEvent*);
  virtual void finalize();
private:

};

#endif
