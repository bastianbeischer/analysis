#include "DataInterface.hh"

#include "AlignmentMatrix.hh"
#include "Hit.hh"
#include "Cluster.hh"
#include "SimpleEvent.hh"
#include "Layer.hh"
#include "CenteredBrokenLine.hh"
#include "DataChain.hh"
#include "TrackSelection.hh"
#include "TrackFinding.hh"
#include "Manager.hh"
#include "Setup.hh"

#include "millepede.h"

#include <iostream>

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

void DataInterface::process(AlignmentMatrix* matrix)
{
  Setup* setup = Setup::instance();
  
  unsigned int nEntries = m_chain->nEntries();
  std::cout << std::endl;
  std::cout << "+----------------------------------------------------------------------------------------------------+" << std::endl;
  std::cout << "| Processing:                                                                                        |" << std::endl;
  std::cout << "| 0%     10%       20%       30%       40%       50%       60%       70%       80%       90%     100%|" << std::endl;
  std::cout << "|.........|.........|.........|.........|.........|.........|.........|.........|.........|..........|" << std::endl;
  std::cout << "|" << std::flush;
  int iFactors = 0;

  for (unsigned int i = 0; i < nEntries; i++) {
    SimpleEvent* event = m_chain->event(i);
    
    QVector<Hit*> clusters;
    QVector<Hit*> hits = QVector<Hit*>::fromStdVector(event->hits());
    if (event->contentType() == SimpleEvent::Clusters)
      clusters = hits;
    else
      clusters = setup->generateClusters(hits);

    // track finding
    clusters = m_trackFinding->findTrack(clusters);

    Track* track = new CenteredBrokenLine;
    TrackSelection selection;
    if (track->fit(clusters)) {
      selection.processTrack(track);
      TrackSelection::Flags flags = selection.flags();
      if ( (flags & TrackSelection::AllTrackerLayers) &&
          !(flags & TrackSelection::MagnetCollision) ) {
        matrix->fillMatrixFromTrack(track);
        FITLOC();
      }
    }
    else {
      delete track;
    }
    
    if (event->contentType() == SimpleEvent::RawData)
      qDeleteAll(clusters);

    if ( i > iFactors*nEntries/100. ) {
      std::cout << "#" << std::flush;
      iFactors++;
    }
  }

  std::cout << "|" << std::endl;
  std::cout << "+----------------------------------------------------------------------------------------------------+" << std::endl;
}
