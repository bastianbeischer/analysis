#include "Layer.hh"

#include <cstdio>
#include <iostream>
#include <sstream>
#include <cassert>

#include "Cluster.hh"
#include "Setup.hh"
#include "SipmArray.hh"
#include "Hit.hh"

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
  assert(element);
  element->addHit(hit);
}

void Layer::clearHitsInDetectors()
{
  foreach(DetectorElement* element, m_elements)
    element->clearHits();
}

void Layer::sortHits()
{
  foreach(DetectorElement* element, m_elements)
    element->sortHits();
}

QVector<Cluster*> Layer::clusters() const
{
  QVector<Cluster*> allClusters;
  foreach(DetectorElement* element, m_elements)
    allClusters += element->findClusters();

  return allClusters;
}

Cluster* Layer::bestCluster() const
{
  QVector<Cluster*> allClusters = clusters();

  int maxAmplitude = 0;
  Cluster* bestCluster = 0;
  for (QVector<Cluster*>::iterator it = allClusters.begin(); it != allClusters.end(); ++it) {
    Cluster* cluster = (*it);
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

const char* Layer::printInfo()
{
  std::stringstream stream;

  stream << nElements() << "elements: ";
  foreach(DetectorElement* element, m_elements)
    stream << element->nHits() << " ";

  return stream.str().c_str();
}

QList<unsigned short> Layer::detIds()
{
  QList<unsigned short> detIds;
  foreach(DetectorElement* element, m_elements)
    detIds.push_back(element->id());
  return detIds;
}
