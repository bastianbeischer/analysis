#include "TOFBar.hh"

#include "TOFCluster.hh"

TOFBar::TOFBar() :
  DetectorElement()
{
  m_type = tof;
}

TOFBar::~TOFBar()
{
}

QVector<Cluster*> TOFBar::findClusters() const
{
  QVector<Cluster*> clusters;
  TOFCluster* tofCluster = new TOFCluster;
  foreach(Hit* hit, m_hits) {
    tofCluster->addHit(hit);
  }
  tofCluster->processHits();
  clusters.push_back(tofCluster);
  return clusters;
}
