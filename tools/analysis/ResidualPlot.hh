#ifndef ResidualPlot_hh
#define ResidualPlot_hh

#include <QMap>

#include "AnalysisPlot.hh"
#include "H2DPlot.hh"

class Layer;
class Particle;

class ResidualPlot : public AnalysisPlot, public H2DPlot {
  
public:
  ResidualPlot(AnalysisPlot::Topic topic, Layer* layer);
  ~ResidualPlot();
  
public:
  void processEvent(const QVector<Hit*>&, Particle* = 0, SimpleEvent* = 0);

private:
  const Layer* m_layer;

};

#endif /* ResidualPlot_hh */
