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

  foreach(Hit* rawhit, rawhits) {
    unsigned short id = rawhit->detId() - rawhit->channel();
    DetectorElement* element = setup->element(id);
    (*hitStorage)[element].append(rawhit);
  }
  
  const HitStorageIterator endIt = hitStorage->end();
  for (HitStorageIterator it = hitStorage->begin(); it != endIt; ++it) {
    DetectorElement* element = it.key();
    const QVector<Hit*>& hits = it.value();
    clusters += element->findClusters(hits);
  }

  delete hitStorage;

  return clusters;
}
