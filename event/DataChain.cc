#include "DataChain.hh"

#include <TFile.h>
#include <TList.h>

#include "SimpleEvent.hh"
#include "DataDescription.hh"

#include <fstream>
#include <iostream>

DataChain::DataChain()
  : m_chain(0)
  , m_event(0)
{
  init();
}

DataChain::DataChain(const char* listName)
  : m_chain(0)
  , m_event(0)
{
  setFileList(listName);
}

DataChain::~DataChain()
{
  delete m_chain;
  delete m_event;
}

void DataChain::init()
{
  if (m_event)
    delete m_event;
  if (m_chain)
    delete m_chain;
  m_chain = new TChain("SimpleEventTree");
  m_event = 0;
  m_currentEntry = -1;
  m_chain->SetBranchAddress("event", &m_event);
  m_chain->SetCacheSize(100e6); // 100MB
  m_chain->AddBranchToCache("*");
}

void DataChain::setFileList(const char* listName)
{
  init();
  addFileList(listName);
}

void DataChain::addFileList(const char* listName)
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
    m_chain->AddFile(filename);

    // version string
    TFile file(filename, "READ");
    TTree* tree = (TTree*)file.Get("SimpleEventTree");
    int nEntries = tree->GetEntries();
    if (m_offsets.size() == 0)
      m_offsets.push_back(nEntries);
    else 
      m_offsets.push_back(nEntries + m_offsets.back());
    DataDescription* desc = (DataDescription*) tree->GetUserInfo()->First();
    if (desc) {
      std::cout << " (version: " << desc->softwareVersionHash() << ")" << std::endl;
    }
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

const DataDescription* DataChain::currentDescription() const
{
  return (const DataDescription*) m_chain->GetTree()->GetUserInfo()->First();
}

int DataChain::entryInFile() const
{
  int entryInFile = m_currentEntry;

  for (unsigned int i = 0; i < m_offsets.size() - 1; i++) {
    if (m_currentEntry >= m_offsets[i] && m_currentEntry < m_offsets[i+1]) {
      entryInFile -= m_offsets[i];
      break;
    }
  }
  assert(entryInFile >= 0);
  return entryInFile;
}

const std::string& DataChain::rawFileNameForEvent() const
{
  return currentDescription()->runFileNameForEventNumber(entryInFile());
}
