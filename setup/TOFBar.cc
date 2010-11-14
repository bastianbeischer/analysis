#include "TOFBar.hh"

#include "TOFCluster.hh"

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

QVector<Cluster*> TOFBar::findClusters()
{
  if(m_hits.size() >= 3) {
    TOFCluster* tofCluster = new TOFCluster;
    foreach(Hit* hit, m_hits) {
      tofCluster->addHit(new Hit(*hit));
    }
    tofCluster->processHits();
    m_clusters.push_back(tofCluster);
  }
  return m_clusters;
}
