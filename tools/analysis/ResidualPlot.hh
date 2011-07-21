#ifndef ResidualPlot_hh
#define ResidualPlot_hh

#include <QMap>

#include "AnalysisPlot.hh"
#include "H2DProjectionPlot.hh"

class Layer;
class Particle;
class Track;

class ResidualPlot : public AnalysisPlot, public H2DProjectionPlot {
  
public:
  ResidualPlot(AnalysisPlot::Topic topic, Layer* layer);
  ~ResidualPlot();
  
public:
  void processEvent(const QVector<Hit*>&, const Particle* const, const SimpleEvent* const);

private:
  virtual Track* referenceTrack(const QVector<Hit*>&, const Particle* const = 0, const SimpleEvent* const = 0);

private:
  const Layer* m_layer;

};

#endif /* ResidualPlot_hh */
