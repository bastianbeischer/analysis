#include "DataInterface.hh"

#include "DataChain.hh"
#include "TrackSelection.hh"


DataInterface::DataInterface() :
  m_chain(new DataChain),
  m_trackSelection(new TrackSelection)
{
}

DataInterface::~DataInterface()
{
  delete m_chain;
  delete m_trackSelection;
}

void DataInterface::addFiles(const char* listName)
{
  m_chain->addFiles(listName);
}

void DataInterface::addSuitableTracks()
{
  
}
