#include "TOFBar.hh"

#include "TOFCluster.hh"
#include "TOFSipmHit.hh"
#include <iostream>

TOFBar::TOFBar() :
  DetectorElement()
{
  m_type = tof;
}

TOFBar::TOFBar(unsigned short detId) :
  DetectorElement(detId)
{
  m_type = tof;
}

TOFBar::~TOFBar()
{
}

QVector<Cluster*> TOFBar::findClusters() const
{
  QVector<Cluster*> clusters;
  if(m_hits.size() != 0) {
    TOFCluster* tofCluster = new TOFCluster;
    foreach(Hit* hit, m_hits) {
      tofCluster->addHit(hit);
    }
    tofCluster->processHits();
    clusters.push_back(tofCluster);
  }
  return clusters;
}
