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

void Layer::addHitToDetector(Hit* hit)
{
  unsigned short detId = hit->detId() - hit->channel();
  DetectorElement* element = m_elements[detId];
  Q_ASSERT(element);
  element->addHit(hit);
}

void Layer::clearHitsInDetectors()
{
  foreach(DetectorElement* element, m_elements)
    element->clearHits();
}

QVector<Hit*> Layer::clusters() const
{
  QVector<Hit*> allClusters;
  foreach(DetectorElement* element, m_elements)
    allClusters += element->findClusters();

  return allClusters;
}

Cluster* Layer::bestCluster() const
{
  QVector<Hit*> allClusters = clusters();

  int maxAmplitude = 0;
  Cluster* bestCluster = 0;
  for (QVector<Hit*>::iterator it = allClusters.begin(); it != allClusters.end(); ++it) {
    Cluster* cluster = static_cast<Cluster*>((*it));
    int amplitude = cluster->signalHeight();
    if (amplitude > maxAmplitude) {
      maxAmplitude = amplitude;
      bestCluster = cluster;
    }
  }

  return bestCluster;
}

bool Layer::contains(unsigned short detId) const
{
  if (m_elements[detId] != 0)
    return true;
  return false;
}

const char* Layer::printInfo() const
{
  std::stringstream stream;

  stream << nElements() << "elements: ";
  foreach(DetectorElement* element, m_elements)
    stream << element->nHits() << " ";

  return stream.str().c_str();
}

void Layer::sortIdsByPosition()
{
  QList<DetectorElement*> elements = m_elements.values();
  qSort(elements.begin(), elements.end(), compareDetectorPosition);
  foreach(DetectorElement* element, elements)
    m_ids.push_back(element->id());
}
