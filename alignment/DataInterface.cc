#include "DataInterface.hh"

#include "Hit.hh"
#include "Cluster.hh"
#include "SimpleEvent.hh"
#include "Layer.hh"
#include "Track.hh"
#include "DataChain.hh"
#include "TrackSelection.hh"
#include "TrackFinding.hh"
#include "Manager.hh"
#include "Setup.hh"

DataInterface::DataInterface() :
  m_chain(new DataChain),
  m_trackSelection(new TrackSelection),
  m_trackFinding(new TrackFinding)
{
}

DataInterface::~DataInterface()
{
  delete m_chain;
  delete m_trackSelection;
  delete m_trackFinding;
}

void DataInterface::addFiles(const char* listName)
{
  m_chain->addFiles(listName);
}

void DataInterface::addSuitableTracks()
{
  Manager* manager = Manager::instance();
  Setup*   setup   = Setup::instance();
  
  for (unsigned int i = 0; i < m_chain->nEntries(); i++) {
    SimpleEvent* event = m_chain->event(i);
    
    // vector of all hits in this event
    QVector<Hit*> hits;
    foreach(Hit* hit, event->hits())
      hits.push_back(hit);

    // add hits to the detectors
    foreach(Hit* hit, hits) {
      if (hit->type() == Hit::tracker || hit->type() == Hit::trd) {
        double z = hit->position().z();
        Layer* layer = setup->layer(z);
        layer->addHitToDetector(hit);
      }
    }

    // find clusters (currently TRD and Tracker)
    QVector<Hit*> clusters;
    Layer* layer = setup->firstLayer();
    while(layer) {
    
      // QVector<Cluster*> clustersHere = layer->clusters();
      // foreach(Cluster* cluster, clustersHere)
      //   clusters.push_back(cluster);

      Cluster* cluster = layer->bestCluster();
      if (cluster)
        clusters.push_back(cluster);
      layer->clearHitsInDetectors();

      // update pointer
      layer = setup->nextLayer();
    }

    // track finding
    clusters = m_trackFinding->findTrack(clusters);

    Track* track = new Track;
    if (track->fit(clusters) && m_trackSelection->passes(track)) {
      manager->addTrack(track);
    }
    else {
      delete track;
    }
    
  }
  
}
