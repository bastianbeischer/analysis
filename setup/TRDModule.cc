#include "TRDModule.hh"

#include "Hit.hh"
#include "Cluster.hh"

TRDModule::TRDModule() :
  DetectorElement()
{
  m_type = trd;
}

TRDModule::TRDModule(unsigned short detId) :
  DetectorElement(detId)
{
  m_type = trd;
}

TRDModule::~TRDModule()
{
}

QVector<Cluster*> TRDModule::findClusters()
{
  const int seedThreshold = 10;
  const int neighbourThreshold = 10;

  for (unsigned short channel = 0; channel < 16; ++channel) {
    if (m_hits[channel]) {
      Hit* hit = m_hits[channel];
      
      if (hit->signalHeight() > seedThreshold) {
        Cluster* cluster = new Cluster;
        cluster->addHit(new Hit(*hit));

        // look to the right
        short rightCursor = channel+1;
        while(rightCursor < 16 && m_hits[rightCursor] && m_hits[rightCursor]->signalHeight() > neighbourThreshold) {
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
        m_clusters.push_back(cluster);

        channel = rightCursor + 1;
      }
    }
  }

  return m_clusters;
}

