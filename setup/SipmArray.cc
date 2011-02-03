#include "SipmArray.hh"

#include "Cluster.hh"
#include "Hit.hh"
#include "Setup.hh"

#include <QString>

SipmArray::SipmArray(unsigned short detId) :
  DetectorElement(tracker, detId, 32)
{
}

SipmArray::~SipmArray()
{
}

QVector<Hit*> SipmArray::findClusters()
{
  QVector<Hit*> clusters;

  const int seedThreshold = 300;
  const int neighbourThreshold = 100;

  for (unsigned short channel = 0; channel < m_nChannels; ++channel) {
    Hit* hit = m_hits[channel];
      
    if (hit->signalHeight() > seedThreshold) {
      Cluster* cluster = new Cluster;
      cluster->addHit(new Hit(*hit));

      // look to the right
      short rightCursor = channel+1;
      while(rightCursor < m_nChannels && m_hits[rightCursor] && m_hits[rightCursor]->signalHeight() > neighbourThreshold) {
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
