#ifndef TOTLayer_hh
#define TOTLayer_hh

#include <AnalysisPlot.hh>

#include <QString>

class TOTLayer : public AnalysisPlot
{
public:
  enum Layer {Lower, Upper, All};
  TOTLayer(Layer type);
  virtual ~TOTLayer() { };
  virtual TOTLayer* create(Layer layer) const = 0;
protected:
  Layer m_layer;
  QString layerName(Layer type);
  bool checkLayer(double z);
};

#endif /* TOTLayer_hh */