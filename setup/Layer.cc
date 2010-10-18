#include "Layer.hh"

#include <cstdio>
#include <iostream>
#include <sstream>

#include "Cluster.hh"
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
  for (std::map<unsigned short, SiPMArray*>::iterator it = m_arrays.begin(); it != m_arrays.end(); it++)
    delete it->second;
}

void Layer::addHitToArray(Hit* hit)
{
  unsigned short detId = hit->detId() - hit->channel();
  if (!m_arrays[detId])
    m_arrays[detId] = new SiPMArray(detId);
  m_arrays[detId]->addHit(hit);
}

void Layer::clearArrays()
{
  for (std::map<unsigned short, SiPMArray*>::iterator it = m_arrays.begin(); it != m_arrays.end(); it++) {
    it->second->clearHits();
  }
}

std::vector<Cluster*> Layer::clusters()
{
  std::vector<Cluster*> allClusters;
  for (std::map<unsigned short, SiPMArray*>::iterator it = m_arrays.begin(); it != m_arrays.end(); it++) {
    std::vector<Cluster*> localClusters = it->second->findClusters();
    for (unsigned int i = 0; i < localClusters.size(); i++)
      allClusters.push_back(localClusters.at(i));
  }
  return allClusters;
}

Cluster* Layer::bestCluster()
{
  std::vector<Cluster*> allClusters = clusters();

  int maxAmplitude = 0;
  Cluster* bestCluster = 0;
  for (std::vector<Cluster*>::iterator it = allClusters.begin(); it != allClusters.end(); it++) {
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
  for (std::map<unsigned short, SiPMArray*>::iterator it = m_arrays.begin(); it != m_arrays.end(); it++) {
    stream << it->second->nHits() << " ";
  }  

  return stream.str().c_str();
}
