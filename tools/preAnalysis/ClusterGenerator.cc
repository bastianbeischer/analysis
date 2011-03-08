#include "ClusterGenerator.hh"

#include "Setup.hh"
#include "Cluster.hh"
#include "TOFCluster.hh"
#include "Hit.hh"
#include "TOFSipmHit.hh"
#include "SimpleEvent.hh"
#include "EventDestination.hh"
#include "DetectorElement.hh"

#include <iostream>

ClusterGenerator::ClusterGenerator() :
  EventProcessor()
{
}

ClusterGenerator::~ClusterGenerator()
{
}

void ClusterGenerator::process(SimpleEvent* sourceEvent)
{
  SimpleEvent* destinationEvent = new SimpleEvent(sourceEvent->eventId(), sourceEvent->runStartTime(), sourceEvent->eventTime(), sourceEvent->contentType());
  if (sourceEvent->contentType() == SimpleEvent::MonteCarlo){
    MCEventInformation* info = new MCEventInformation(*sourceEvent->MCInformation());
    destinationEvent->setMCInformation(info);
  }

  Setup* setup = Setup::instance();

  // vector of all hits in this event
  QVector<Hit*> rawhits = QVector<Hit*>::fromStdVector(sourceEvent->hits());

  // distribute raw hits
  foreach(Hit* rawhit, rawhits) {
    unsigned short id = rawhit->detId() - rawhit->channel();
    DetectorElement* element = setup->element(id);
    QMap<unsigned short, Hit*>& hitMap = m_hitStorage[element];
    hitMap[element->sortedChannel(rawhit->channel())] = rawhit;
  }

  // do the zero suppression
  const HitStorageIterator hitEndIt = m_hitStorage.end();
  for (HitStorageIterator it = m_hitStorage.begin(); it != hitEndIt; ++it) {
    DetectorElement* element = it.key();
    const QVector<Hit*>& hits = it.value().values().toVector();
    foreach (Hit* cluster, element->findClusters(hits))
      destinationEvent->addHit(cluster);
    it.value().clear();
  }

  // copy sensor  data
  for (unsigned int i = SensorTypes::START; i < SensorTypes::END; i++) {
    destinationEvent->setSensorData((SensorTypes::Type)i, sourceEvent->sensorData((SensorTypes::Type)i) );
  }

  foreach(EventDestination* destination, m_destinations)
    destination->processEvent(rawhits, 0, destinationEvent);
}
