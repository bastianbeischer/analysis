#include "DataInterface.hh"

#include "AlignmentMatrix.hh"
#include "Hit.hh"
#include "Cluster.hh"
#include "SimpleEvent.hh"
#include "Layer.hh"
#include "CenteredBrokenLine.hh"
#include "DataChain.hh"
#include "TrackFinding.hh"
#include "Manager.hh"
#include "Setup.hh"
#include "Corrections.hh"
#include "AnalysisProcessor.hh"

#include "millepede.h"

#include <iostream>

DataInterface::DataInterface() :
  m_chain(new DataChain),
  m_processor(new AnalysisProcessor)
{
  m_processor->setTrackType(Track::CenteredBrokenLine);
}

DataInterface::~DataInterface()
{
  delete m_chain;
}

void DataInterface::addFiles(const char* listName)
{
  m_chain->addFileList(listName);
}

void DataInterface::process(AlignmentMatrix* matrix)
{
  m_processor->clearDestinations();
  m_processor->addDestination(matrix);

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
    m_processor->process(event);
    delete event;
    FITLOC();
    
    if ( i > iFactors*nEntries/100. ) {
      std::cout << "#" << std::flush;
      iFactors++;
    }
  }

  std::cout << "|" << std::endl;
  std::cout << "+----------------------------------------------------------------------------------------------------+" << std::endl;
}
