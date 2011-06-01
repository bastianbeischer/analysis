#include "TOTLayer.hh"

TOTLayer::TOTLayer(Layer layer) : AnalysisPlot(TimeOverThreshold)
  ,m_layer(layer)
{
}

QString TOTLayer::layerName(Layer layer)
{
  switch (layer) {
    case Lower: return "lower";
    case Upper: return "upper";
    case All:   return "total";
  }
  return QString();
}

bool TOTLayer::checkLayer(double z)
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

QString TOTLayer::getPlotName()
{
  return m_plotName;
}