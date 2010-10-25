#include "DataChain.hh"

#include <TFile.h>
#include <TList.h>

#include "SimpleEvent.hh"
#include "DataDescription.hh"

#include <fstream>
#include <iostream>

DataChain::DataChain()
{
  init();
}

DataChain::DataChain(const char* listName)
{
  init();
  addFiles(listName);
}

DataChain::~DataChain()
{
  delete m_chain;
  delete m_event;
}

void DataChain::init()
{
  m_chain = new TChain("SimpleEventTree");
  m_event = 0;
  m_currentEntry = -1;
  m_chain->SetBranchAddress("event", &m_event);
}

void DataChain::addFiles(const char* listName)
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

SimpleEvent* DataChain::event(unsigned int i) 
{
  assert(i < m_chain->GetEntries());
  m_currentEntry = i;
  m_chain->GetEntry(i); 
  return m_event;
}

SimpleEvent* DataChain::nextEvent() 
{
  m_currentEntry++;
  if (m_currentEntry >= m_chain->GetEntries())
    return 0;
  m_chain->GetEntry(m_currentEntry); 
  return m_event;
}
