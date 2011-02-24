#include "ClusterGenerator.hh"

#include "Setup.hh"
#include "Hit.hh"
#include "DetectorElement.hh"

ClusterGenerator::ClusterGenerator()
{
}

ClusterGenerator::~ClusterGenerator()
{
}

QVector<Hit*> ClusterGenerator::findClusters(const QVector<Hit*>& rawhits)
{
  QVector<Hit*> clusters;

  Setup* setup = Setup::instance();

  HitStorage* hitStorage = new HitStorage;

  const ElementIterator endIt = setup->lastElement();
  for (ElementIterator it = setup->firstElement(); it != endIt; ++it) {
    DetectorElement* element = *it;
    for (int i = 0; i < element->nChannels(); i++) {
      (*hitStorage)[element].append(0);
    }
  }

  foreach(Hit* rawhit, rawhits) {
    unsigned short id = rawhit->detId() - rawhit->channel();
    DetectorElement* element = setup->element(id);
    QVector<Hit*>& hitVector = (*hitStorage)[element];
    hitVector[element->sortedChannel(rawhit->channel())] = rawhit;
  }
  
  const HitStorageIterator hitEndIt = hitStorage->end();
  for (HitStorageIterator it = hitStorage->begin(); it != hitEndIt; ++it) {
    DetectorElement* element = it.key();
    const QVector<Hit*>& hits = it.value();
    clusters += element->findClusters(hits);
  }

  delete hitStorage;

  return clusters;
}
