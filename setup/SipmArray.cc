#include "SipmArray.hh"

#include "Cluster.hh"
#include "Hit.hh"
#include "Setup.hh"

#include <QString>

SipmArray::SipmArray(unsigned short detId, const Setup* setup) :
  DetectorElement(tracker, detId, 32, setup)
{
}

SipmArray::~SipmArray()
{
}

QVector<Hit*> SipmArray::findClusters(QVector<Hit*>& rawhits)
{
  QVector<Hit*> clusters;

  const int seedThreshold = 300;
  const int neighbourThreshold = 100;

  for (unsigned short channel = 0; channel < m_nChannels; ++channel) {
    Hit* hit = rawhits[channel];
    Q_ASSERT(hit);
      
    if (hit->signalHeight() > seedThreshold) {
      Cluster* cluster = new Cluster;

      // look to the right
      short rightCursor = channel+1;
      while(rightCursor < m_nChannels && rawhits[rightCursor]->signalHeight() > neighbourThreshold)
        ++rightCursor;

      // look to the left
      short leftCursor = channel-1;
      while(leftCursor >=0 && rawhits[leftCursor]->signalHeight() > neighbourThreshold)
        --leftCursor;

      for (int channelToBeAdded = leftCursor+1; channelToBeAdded < rightCursor; channelToBeAdded++)
        cluster->addHit(new Hit(*rawhits[channelToBeAdded]));

      cluster->processHits();
      clusters.push_back(cluster);

      channel = rightCursor + 1;
    }
  }

  return clusters;
}
