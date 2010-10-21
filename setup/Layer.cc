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

void Layer::addHitToArray(Hit* hit)
{
  unsigned short detId = hit->detId() - hit->channel();
  Setup* setup = Setup::instance();
  SipmArray* array = static_cast<SipmArray*>(setup->element(detId));
  array->addHit(hit);
  m_arrays.push_back(array);
}

void Layer::clearArrays()
{
  foreach(SipmArray* array, m_arrays)
    array->clearHits();
}

QVector<Cluster*> Layer::clusters() const
{
  QVector<Cluster*> allClusters;
  foreach(SipmArray* array, m_arrays) {
    QVector<Cluster*> localClusters = array->findClusters();
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

  stream << nArrays() << "arrays: ";
  foreach(SipmArray* array, m_arrays)
    stream << array->nHits() << " ";

  return stream.str().c_str();
}
