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
  ResidualPlot(Enums::AnalysisTopic topic, Layer* layer);
  ~ResidualPlot();
  
public:
  void processEvent(const AnalyzedEvent*);

private:
  virtual Track* referenceTrack(const AnalyzedEvent*);

private:
  const Layer* m_layer;

};

#endif /* ResidualPlot_hh */
