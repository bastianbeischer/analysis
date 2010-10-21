#include "Layer.hh"

#include <cstdio>
#include <iostream>
#include <sstream>

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
  Setup* setup = Setup::instance();
  DetectorElement* element = setup->element(detId);
  element->addHit(hit);
  m_elements.push_back(element);
}

void Layer::clearHitsInDetectors()
{
  foreach(DetectorElement* element, m_elements)
    element->clearHits();
}

QVector<Cluster*> Layer::clusters() const
{
  QVector<Cluster*> allClusters;
  foreach(DetectorElement* element, m_elements) {
    QVector<Cluster*> localClusters = element->findClusters();
    foreach(Cluster* cluster, localClusters)
      allClusters.push_back(cluster);
  }
  return allClusters;
}

Cluster* Layer::bestCluster() const
{
  QVector<Cluster*> allClusters = clusters();

  int maxAmplitude = 0;
  Cluster* bestCluster = 0;
  for (QVector<Cluster*>::iterator it = allClusters.begin(); it != allClusters.end(); it++) {
    Cluster* cluster = (*it);
    int amplitude = cluster->signalHeight();
    if (amplitude > maxAmplitude) {
      maxAmplitude = amplitude;
      bestCluster = cluster;
    }
  }

  return bestCluster;
}

const char* Layer::printInfo()
{
  std::stringstream stream;

  stream << nElements() << "elements: ";
  foreach(DetectorElement* element, m_elements)
    stream << element->nHits() << " ";

  return stream.str().c_str();
}
