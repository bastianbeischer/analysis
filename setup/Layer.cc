#include "Layer.hh"

#include <cstdio>
#include <iostream>
#include <sstream>

#include "Cluster.hh"
#include "Setup.hh"
#include "SipmArray.hh"
#include "Hit.hh"

static bool compareDetectorPosition(const DetectorElement* element1, const DetectorElement* element2)
{
  return element1->position().x() <= element2->position().x();
}

Layer::Layer() :
  m_z(0)
{
}

Layer::Layer(double z) :
  m_z(z)
{
}

Layer::~Layer()
{
}

bool Layer::contains(unsigned short detId) const
{
  return (m_elements[detId] != 0);
}

void Layer::sortIdsByPosition()
{
  QList<DetectorElement*> elements = m_elements.values();
  qSort(elements.begin(), elements.end(), compareDetectorPosition);
  foreach(DetectorElement* element, elements)
    m_ids.push_back(element->id());
}
