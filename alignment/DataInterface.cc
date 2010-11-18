#include "DataInterface.hh"

#include "Hit.hh"
#include "Cluster.hh"
#include "SimpleEvent.hh"
#include "Layer.hh"
#include "StraightLine.hh"
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
  m_chain->addFileList(listName);
}

void DataInterface::addSuitableTracks()
{
  Manager* manager = Manager::instance();
  Setup* setup = Setup::instance();
  
  for (unsigned int i = 0; i < m_chain->nEntries(); i++) {
    SimpleEvent* event = m_chain->event(i);
    
    QVector<Hit*> hits = QVector<Hit*>::fromStdVector(event->hits());
    QVector<Hit*> clusters;
    foreach(Cluster* cluster, setup->generateClusters(hits))
      clusters.push_back(cluster);

    // track finding
    clusters = m_trackFinding->findTrack(clusters);

    Track* track = new StraightLine;
    if (track->fit(clusters)) {
      manager->addTrack(track);
    }
    else {
      delete track;
    }
    
  }
}
