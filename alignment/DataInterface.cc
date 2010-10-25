#include "DataInterface.hh"

#include "TrackSelection.hh"

DataInterface::DataInterface() :
  m_trackSelection(new TrackSelection)
{
}

DataInterface::~DataInterface()
{
  delete m_trackSelection;
}

