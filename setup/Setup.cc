#include "Setup.hh"

#include "Layer.hh"
#include "DetectorElement.hh"
#include "SiPMArray.hh"

Setup* Setup::m_instance = 0;

Setup::Setup() :
  m_layerIt(0),
  m_elementIt(0)
{
}

Setup::~Setup()
{
  foreach(Layer* layer, m_layers)
    delete layer;
  foreach(DetectorElement* element, m_elements)
    delete element;
}

Setup* Setup::instance()
{
  if (!m_instance) m_instance = new Setup;
  return m_instance;
}

Layer* Setup::firstLayer()
{
  if (m_layers.size() == 0) return 0;
  m_layerIt = m_layers.begin();
  return m_layerIt.value();
}

DetectorElement* Setup::firstElement()
{
  if (m_elements.size() == 0) return 0;
  m_elementIt = m_elements.begin();
  return m_elementIt.value();
}

Layer* Setup::nextLayer()
{
  m_layerIt++;
  if (m_layerIt == m_layers.end())
    return 0;

  return m_layerIt.value();
}

DetectorElement* Setup::nextElement()
{
  m_elementIt++;
  if (m_elementIt == m_elements.end())
    return 0;

  return m_elementIt.value();
}

// this should differentiate between types of detector elements soon
Layer* Setup::layer(double z)
{
  if (!m_layers[z]) m_layers[z] = new Layer;
  return m_layers[z];
}

// this should differentiate between types of detector elements soon
DetectorElement* Setup::element(unsigned short id)
{
  if (!m_elements[id]) m_elements[id] = new SiPMArray;
  return m_elements[id];
}
