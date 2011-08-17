#include "PreAnalysis.hh"

#include "Setup.hh"
#include "Cluster.hh"
#include "TOFCluster.hh"
#include "Hit.hh"
#include "TOFSipmHit.hh"
#include "SimpleEvent.hh"
#include "DetectorElement.hh"

#include <iostream>

PreAnalysis::PreAnalysis()
{
  Setup* setup = Setup::instance();
  const ElementIterator elementEndIt = setup->lastElement();
  for (ElementIterator elementIt = setup->firstElement(); elementIt != elementEndIt; ++elementIt) {
    DetectorElement* element = *elementIt;
    int nChannels = element->nChannels();
    for (int i = 0; i < nChannels; i++) {
      m_hitStorage[element].append(0);
    }
  }  
}

PreAnalysis::~PreAnalysis()
{
}

SimpleEvent* PreAnalysis::generateCompressedEvent(SimpleEvent* sourceEvent)
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
    unsigned short id = rawhit->elementId();
    DetectorElement* element = setup->element(id);
    QVector<Hit*>& hits = m_hitStorage[element];
    hits[element->sortedChannel(rawhit->channel())] = rawhit;
  }

  // do the zero suppression
  const HitStorageIterator hitEndIt = m_hitStorage.end();
  for (HitStorageIterator it = m_hitStorage.begin(); it != hitEndIt; ++it) {
    DetectorElement* element = it.key();
    QVector<Hit*>& hits = m_hitStorage[element];
    const QVector<Hit*>& clusters = element->findClusters(hits);
    const QVector<Hit*>::const_iterator clustersEnd = clusters.end();
    for (QVector<Hit*>::const_iterator it = clusters.begin(); it != clustersEnd; ++it)
      destinationEvent->addHit(*it);

    int nChannels = element->nChannels();
    for (int i = 0; i < nChannels; i++)
      hits[i] = 0;
  }

  // copy sensor  data
  for (unsigned int i = 0; i < SensorTypes::N_SENSOR_TYPES; i++) {
    destinationEvent->setSensorData((SensorTypes::Type)i, sourceEvent->sensorData((SensorTypes::Type)i) );
  }
  
  delete sourceEvent;

  return destinationEvent;
}
