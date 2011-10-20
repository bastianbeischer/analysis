#ifndef TOTLayerPlot_hh
#define TOTLayerPlot_hh

#include "AnalysisPlot.hh"
#include "H1DPlot.hh"
#include "TOTLayer.hh"

#include <QVector>
//#include <QString>

class Hit;
class Particle;
class SimpleEvent;

class TOTLayerPlot : public H1DPlot, public TOTLayer {
public:
  TOTLayerPlot();
  TOTLayerPlot(TOTLayer::Layer layer);
  ~TOTLayerPlot();
  TOTLayerPlot* create(TOTLayer::Layer layer) const;
  virtual void processEvent(const AnalyzedEvent*);
private:

  
};

#endif /* TOTLayerPlot_hh */
