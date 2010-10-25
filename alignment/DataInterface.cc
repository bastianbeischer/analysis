#include "DataInterface.hh"

#include "DataDescription.hh"
#include "TrackSelection.hh"

#include <fstream>
#include <TFile.h>
#include <TList.h>
#include <TChain.h>
#include <TTree.h>

#include <iostream>

DataInterface::DataInterface() :
  m_trackSelection(new TrackSelection)
{
}

DataInterface::~DataInterface()
{
  delete m_trackSelection;
}

void DataInterface::addFiles(const char* listName)
{
  ifstream file(listName);
  if (!file.is_open()) {
    std::cout << "Error opening file" << std::endl;
    return;
  }
  while (true) {
    // read filename from list
    char filename[256];
    file >> filename;
    if (file.eof()) break;

    // starting output
    std::cout << "Adding " << filename;

    // version string
    TFile file(filename, "READ");
    TTree* tree = (TTree*)file.Get("SimpleEventTree");
    DataDescription* desc = (DataDescription*) tree->GetUserInfo()->First();
    if (desc) 
      std::cout << " (version: " << desc->softwareVersionHash() << ")";
    
    // complete output
    std::cout << std::endl;

    // actually add the file
    m_chain->AddFile(filename);
  }
  
  std::cout << "DONE: Chain contains " << m_chain->GetEntries() << " events" << std::endl;
}
