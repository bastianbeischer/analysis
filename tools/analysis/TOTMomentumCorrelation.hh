#ifndef TOTMomentumCorrelation_hh
#define TOTMomentumCorrelation_hh

#include "H2DPlot.hh"
#include "TOTLayerPlot.hh"

#include <QString>

class Hit;
class Particle;
class SimpleEvent;

class TOTMomentumCorrelation : public H2DPlot, public TOTLayerPlot {
public:
  TOTMomentumCorrelation(const QString& title, TOTLayerPlot::Layer layer);
  ~TOTMomentumCorrelation();
  virtual void processEvent(const AnalyzedEvent*);
  virtual void finalize();
};

#endif
