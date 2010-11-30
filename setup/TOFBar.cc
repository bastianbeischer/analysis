#include "TOFBar.hh"

#include "TOFCluster.hh"
#include "TOFSipmHit.hh"
#include "Setup.hh"

#include <QString>

TOFBar::TOFBar() :
  DetectorElement()
{
  m_type = tof;
  m_nChannels = 4;
}

TOFBar::TOFBar(unsigned short detId) :
  DetectorElement(detId)
{
  m_type = tof;
  m_nChannels = 4;
  m_position = Setup::instance()->configFilePosition("tof", m_id);
  m_alignmentShift = Setup::instance()->configFileAlignmentShift("tof", m_id);
}

TOFBar::~TOFBar()
{
}

QVector<Cluster*> TOFBar::findClusters()
{
  QVector<Cluster*> clusters;
  if(m_hits.size() >= 3) {
    TOFCluster* tofCluster = new TOFCluster;
    foreach(Hit* hit, m_hits) {
      TOFSipmHit* tofHit = static_cast<TOFSipmHit*>(hit);
      tofCluster->addHit(new TOFSipmHit(*tofHit));
    }
    tofCluster->processHits();
    clusters.push_back(tofCluster);
  }
  return clusters;
}

