#include "DataChain.hh"

#include <QMutexLocker>
#include <QRegExp>

#include <TFile.h>
#include <TList.h>
#include <TROOT.h>

#include "SimpleEvent.hh"
#include "DataDescription.hh"
#include "Constants.hh"
#include "Helpers.hh"

#include <cstdlib>
#include <fstream>
#include <iostream>
#include <algorithm>

QMutex DataChain::s_mutex;

DataChain::DataChain()
  : m_chain(0)
  , m_event(0)
  , m_currentEntry(-1)
{
  clear();
}

DataChain::DataChain(const char* listName)
  : m_chain(0)
  , m_event(0)
  , m_currentEntry(-1)
{
  setFileList(listName);
}

DataChain::~DataChain()
{
  delete m_chain;
  for (std::vector<const DataDescription*>::iterator it = m_descriptionBuffer.begin(); it != m_descriptionBuffer.end(); it++) {
    delete *it;
  }
}

void DataChain::clear()
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
  clear();
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
    file.getline(filename,256);
    if (file.eof()) break;
    QString fullname(filename);
    if (fullname.endsWith(".txt"))
      addFileList(qPrintable(fullname));
    else if (fullname.endsWith(".root")) {
      fullname.prepend(Helpers::dataPath() + "/");
      addRootFile(qPrintable(fullname));
    }
  }

  std::cout << "DONE: Chain now contains " << m_chain->GetEntries() << " events in total (after addition of " << listName << ")" << std::endl;
}

void DataChain::addRootFile(const char* filename)
{
  // starting output
  std::cout << "Adding " << filename;
  m_chain->AddFile(filename);

  // version string
  TFile file(filename, "READ");
  gROOT->cd();
  TTree* tree = (TTree*)file.Get("SimpleEventTree");
  int nEntries = tree->GetEntries();
  if (m_offsets.size() == 0)
    m_offsets.push_back(nEntries);
  else
    m_offsets.push_back(nEntries + m_offsets.back());
  std::cout << " with " << nEntries << " events";
  DataDescription* desc = (DataDescription*) tree->GetUserInfo()->First();
  if (desc) {
    m_descriptionBuffer.push_back(new DataDescription(*desc)); // create a copy because the original vanishes when the TChain closes its TFile
    std::cout << " (version: " << desc->softwareVersionHash() << ")" << std::endl;
    for (int i = 0; i < desc->numberOfRuns(); i++) {
      QString name(desc->runFileName(i).c_str());
      QRegExp re("run.*(\\d+)\\.dat");
      if(name.contains(re)) {
        unsigned int time = re.cap(1).toUInt();
        m_runNumbers.push_back(time);
      }
    }
  }
  std::sort(m_runNumbers.begin(), m_runNumbers.end());
}

SimpleEvent* DataChain::event(unsigned int i)
{
  QMutexLocker locker(&s_mutex);
  assert(i < m_chain->GetEntries());
  m_currentEntry = i;
  m_event = 0;
  m_chain->GetEntry(i);
  int tree = m_chain->GetTreeNumber();
  const DataDescription* desc = m_descriptionBuffer[tree];
  assert(desc);
  m_event->setDescription(desc);
  return m_event;
}

SimpleEvent* DataChain::nextEvent()
{
  ++m_currentEntry;
  if (m_currentEntry >= m_chain->GetEntries()) {
    m_currentEntry = -1;
    return 0;
  }
  m_chain->GetEntry(m_currentEntry);
  return m_event;
}

const DataDescription* DataChain::currentDescription() const
{
  return static_cast<const DataDescription*>(m_chain->GetTree()->GetUserInfo()->First());
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

QDateTime DataChain::time(int eventNumber)
{
  int savedEntry = m_currentEntry;
  SimpleEvent* ev = event(eventNumber);
  QDateTime dt;
  if (ev)
    dt.setTime_t(ev->time());
  if (savedEntry >= 0)
    event(savedEntry);
  return dt;
}

QDateTime DataChain::startTime()
{
  return QDateTime::fromTime_t(m_runNumbers.front());
}

QDateTime DataChain::stopTime()
{
  return QDateTime::fromTime_t(m_runNumbers.back() + Constants::runLength);
}
