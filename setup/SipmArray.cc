#include "SipmArray.hh"

#include "Cluster.hh"
#include "Hit.hh"
#include "Setup.hh"

#include <QString>

SipmArray::SipmArray() :
  DetectorElement()
{
  m_type = tracker;
}

SipmArray::SipmArray(unsigned short detId) :
  DetectorElement(detId)
{
  m_type = tracker;
  m_position = Setup::instance()->configFilePosition("tracker", m_id);
  m_alignmentShift = Setup::instance()->configFileAlignmentShift("tracker", m_id);
}

SipmArray::~SipmArray()
{
}

QVector<Cluster*> SipmArray::findClusters()
{
  QVector<Cluster*> clusters;

  const int seedThreshold = 300;
  const int neighbourThreshold = 100;

  for (unsigned short channel = 0; channel < 32; ++channel) {
    Hit* hit = m_hits[channel];
      
    if (hit->signalHeight() > seedThreshold) {
      Cluster* cluster = new Cluster;
      cluster->addHit(new Hit(*hit));

      // look to the right
      short rightCursor = channel+1;
      while(rightCursor < 32 && m_hits[rightCursor] && m_hits[rightCursor]->signalHeight() > neighbourThreshold) {
        cluster->addHit(new Hit(*m_hits[rightCursor]));
        ++rightCursor;
      }
      // look to the left
      short leftCursor = channel-1;
      while(leftCursor >=0 && m_hits[leftCursor] && m_hits[leftCursor]->signalHeight() > neighbourThreshold) {
        cluster->addHit(new Hit(*m_hits[leftCursor]));
        --leftCursor;
      }

      cluster->processHits();
      clusters.push_back(cluster);

      channel = rightCursor + 1;
    }
  }

  return clusters;
}
