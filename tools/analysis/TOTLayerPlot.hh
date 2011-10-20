#ifndef TOTLayerPlot_hh
#define TOTLayerPlot_hh

#include "AnalysisPlot.hh"

class TOTLayerPlot : public AnalysisPlot {
public:
  enum Layer {Lower, Upper, All};

  TOTLayerPlot(Layer layer);
  virtual ~TOTLayerPlot();
protected:
  QString layerName();
  bool checkLayer(double z);

  Layer m_layer;
};

#endif /* TOTLayerPlot_hh */
