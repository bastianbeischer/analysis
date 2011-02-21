#include "TRDModule.hh"

#include "Hit.hh"
#include "Cluster.hh"
#include "Setup.hh"

#include <QString>

TRDModule::TRDModule(unsigned short detId, Setup* setup) :
  DetectorElement(trd, detId, 16, setup)
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
    Hit* hit = m_hits[channel];
    Q_ASSERT(hit);
      
    if (hit->signalHeight() > seedThreshold) {
      Cluster* cluster = new Cluster;

      // look to the right
      short rightCursor = channel+1;
      while(rightCursor < m_nChannels && m_hits[rightCursor]->signalHeight() > neighbourThreshold)
        ++rightCursor;

      // look to the left
      short leftCursor = channel-1;
      while(leftCursor >=0 && m_hits[leftCursor]->signalHeight() > neighbourThreshold)
        --leftCursor;

      for (int channelToBeAdded = leftCursor+1; channelToBeAdded < rightCursor; channelToBeAdded++)
        cluster->addHit(new Hit(*m_hits[channelToBeAdded]));

      cluster->processHits();
      clusters.push_back(cluster);

      channel = rightCursor + 1;
    }
  }

  return clusters;
}
