#ifndef TOTBetaCorrelation_hh
#define TOTBetaCorrelation_hh

#include "AnalysisPlot.hh"
#include "H2DPlot.hh"
#include "TOTLayer.hh"

#include <QString>

class Hit;
class Particle;
class SimpleEvent;

class TOTBetaCorrelation : public H2DPlot, public TOTLayer  {
public:
  TOTBetaCorrelation();
  TOTBetaCorrelation(TOTLayer::Layer layer);
  ~TOTBetaCorrelation();
  TOTBetaCorrelation* create(TOTLayer::Layer layer) const;
  virtual void processEvent(const QVector<Hit*>&, Particle* = 0, SimpleEvent* = 0);
  virtual void finalize();
private:

};

#endif
