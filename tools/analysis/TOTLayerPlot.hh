#ifndef TOTLayerPlot_hh
#define TOTLayerPlot_hh

#include "AnalysisPlot.hh"
#include "H1DPlot.hh"

#include <QVector>
#include <QString>

class Hit;
class Particle;
class SimpleEvent;

class TOTLayerPlot : public AnalysisPlot, public H1DPlot {
public:
  enum TofLayer {Lower, Upper, All};
  TOTLayerPlot(TofLayer layer);
  ~TOTLayerPlot();
  virtual void processEvent(const QVector<Hit*>&, Particle* = 0, SimpleEvent* = 0);
private:
  TofLayer m_layer;
  QString layerName(TofLayer layer);
  bool checkLayer(double z);
};

#endif /* TOTLayerPlot_hh */
