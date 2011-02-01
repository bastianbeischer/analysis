#include "TRDModule.hh"

#include "Hit.hh"
#include "Cluster.hh"
#include "Setup.hh"

#include <QString>

TRDModule::TRDModule(unsigned short detId) :
  DetectorElement(trd, detId, 16)
{
}

TRDModule::~TRDModule()
{
}

QVector<Hit*> TRDModule::findClusters()
{
  QVector<Hit*> clusters;

  const int seedThreshold = 10;
  const int neighbourThreshold = 10;

  for (unsigned short channel = 0; channel < m_nChannels; ++channel) {
    if (m_hits[channel]) {
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
  }

  return clusters;
}
