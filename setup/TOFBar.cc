#include "TOFBar.hh"

#include "TOFCluster.hh"
#include "TOFSipmHit.hh"

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

const QVector<Cluster*>& TOFBar::findClusters()
{
  if(m_hits.size() >= 3) {
    TOFCluster* tofCluster = new TOFCluster;
    foreach(Hit* hit, m_hits) {
      TOFSipmHit* tofHit = static_cast<TOFSipmHit*>(hit);
      tofCluster->addHit(new TOFSipmHit(*tofHit));
    }
    tofCluster->processHits();
    m_clusters.push_back(tofCluster);
  }
  return m_clusters;
}
