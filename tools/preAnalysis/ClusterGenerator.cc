#include "ClusterGenerator.hh"

#include "Setup.hh"
#include "Hit.hh"
#include "DetectorElement.hh"

ClusterGenerator::ClusterGenerator()
{
  // allocate QMap
  Setup* setup = Setup::instance();
  const ElementIterator endIt = setup->lastElement();
  for (ElementIterator it = setup->firstElement(); it != endIt; ++it) {
    DetectorElement* element = *it;
    for (int i = 0; i < element->nChannels(); i++) {
      m_hitStorage[element].append(0);
    }
  }
}

ClusterGenerator::~ClusterGenerator()
{
}

QVector<Hit*> ClusterGenerator::findClusters(const QVector<Hit*>& rawhits)
{
  QVector<Hit*> clusters;

  Setup* setup = Setup::instance();

  // distribute raw hits
  foreach(Hit* rawhit, rawhits) {
    unsigned short id = rawhit->detId() - rawhit->channel();
    DetectorElement* element = setup->element(id);
    QVector<Hit*>& hitVector = m_hitStorage[element];
    hitVector[element->sortedChannel(rawhit->channel())] = rawhit;
  }
  
  // process raw hits
  const HitStorageIterator hitEndIt = m_hitStorage.end();
  for (HitStorageIterator it = m_hitStorage.begin(); it != hitEndIt; ++it) {
    DetectorElement* element = it.key();
    const QVector<Hit*>& hits = it.value();
    clusters += element->findClusters(hits);
  }

  return clusters;
}