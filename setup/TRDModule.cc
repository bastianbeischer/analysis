#include "TRDModule.hh"

#include "Hit.hh"
#include "Cluster.hh"
#include "Setup.hh"

#include <QString>

TRDModule::TRDModule(unsigned short detId, const Setup* setup) :
  DetectorElement(trd, detId, 16, setup)
{
}

TRDModule::~TRDModule()
{
}

QVector<Hit*> TRDModule::findClusters(const QVector<Hit*>& rawhits)
{
  QVector<Hit*> clusters;

  const int seedThreshold = 10;
  const int neighbourThreshold = 10;

  for (unsigned short channel = 0; channel < m_nChannels; ++channel) {
    Hit* hit = rawhits[channel];
      
    if (hit && hit->signalHeight() > seedThreshold) {
      Cluster* cluster = new Cluster;

      // look to the right
      short rightCursor = channel+1;
      while(rightCursor < m_nChannels && rawhits[rightCursor] && rawhits[rightCursor]->signalHeight() > neighbourThreshold)
        ++rightCursor;

      // look to the left
      short leftCursor = channel-1;
      while(leftCursor >=0 && rawhits[leftCursor] && rawhits[leftCursor]->signalHeight() > neighbourThreshold)
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
