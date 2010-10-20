#include "Layer.hh"

#include <cstdio>
#include <iostream>
#include <sstream>

#include "Cluster.hh"
#include "Setup.hh"
#include "SiPMArray.hh"
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
  for (QMap<unsigned short, SiPMArray*>::iterator it = m_arrays.begin(); it != m_arrays.end(); it++)
    delete it.value();
}

void Layer::addHitToArray(Hit* hit)
{
  unsigned short detId = hit->detId() - hit->channel();
  Setup* setup = Setup::instance();
  SiPMArray* array = static_cast<SiPMArray*>(setup->element(detId));
  array->addHit(hit);
  m_arrays[detId] = array;
}

void Layer::clearArrays()
{
  for (QMap<unsigned short, SiPMArray*>::iterator it = m_arrays.begin(); it != m_arrays.end(); it++) {
    it.value()->clearHits();
  }
}

QVector<Cluster*> Layer::clusters()
{
  QVector<Cluster*> allClusters;
  for (QMap<unsigned short, SiPMArray*>::iterator it = m_arrays.begin(); it != m_arrays.end(); it++) {
    QVector<Cluster*> localClusters = it.value()->findClusters();
    foreach(Cluster* cluster, localClusters)
      allClusters.push_back(cluster);
  }
  return allClusters;
}

Cluster* Layer::bestCluster()
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
  for (QMap<unsigned short, SiPMArray*>::iterator it = m_arrays.begin(); it != m_arrays.end(); it++) {
    stream << it.value()->nHits() << " ";
  }  

  return stream.str().c_str();
}
