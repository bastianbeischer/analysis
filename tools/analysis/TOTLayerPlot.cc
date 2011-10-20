#include "TOTLayerPlot.hh"

TOTLayerPlot::TOTLayerPlot(Layer layer)
  : AnalysisPlot(Enums::TimeOverThreshold)
  , m_layer(layer)
{
}

TOTLayerPlot::~TOTLayerPlot()
{
}

QString TOTLayerPlot::layerName()
{
  switch (m_layer) {
    case Lower: return "lower layer";
    case Upper: return "upper layer";
    case All: return "both layers";
  }
  return QString();
}

bool TOTLayerPlot::checkLayer(double z)
{
  if (m_layer == Upper && z > 0) {
    return true;
  } else if (m_layer == Lower && z < 0) {
    return true;
  } else if (m_layer == All) {
    return true;
  }
  return false;
}
