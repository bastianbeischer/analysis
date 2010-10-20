#include "DataDescription.hh"
#include "SimpleEvent.hh"

#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <iostream>
#include <sstream>

#include <TFile.h>
#include <TTree.h>

ClassImp(DataDescription);

DataDescription::DataDescription()
  : m_numberOfRuns(0)
{
  FILE* file = popen("git rev-parse HEAD | tr -d '\n'", "r");
  char line[128];
  std::stringstream stream;
  while (fgets(line, sizeof(line), file)) {
    stream << line;
  }
  pclose(file);
  m_softwareVersionHash = stream.str();
  assert(m_softwareVersionHash.length() == 40);
}

DataDescription::~DataDescription()
{}

int DataDescription::timeOfRun(int) const
{
  //TODO
  return 0;
}
    

long DataDescription::eventNumberInRunFile(long eventNumber) const
{
  //TODO
  return 0;
}

const std::string& DataDescription::runFileForEventNumber(long eventNumber) const
{
  int i = 0;
  for (; i < numberOfRuns(); ++i) {
    if (m_eventNumberOffset[i] >= eventNumber)
      break;
  }
  return m_runFileNames[i];
}

    
void DataDescription::addRunFile(const std::string& fileName)
{
  TFile file(fileName.c_str());
  if (!file.IsZombie()) {
    TTree* tree = static_cast<TTree*>(file.Get("SimpleEventTree"));
    SimpleEvent* event = 0;
    tree->SetBranchAddress("event", &event);
    m_runFileNames.push_back(fileName);
    long nEvents = tree->GetEntries();
    m_eventNumberOffset.push_back(
        m_numberOfRuns == 0 ?
        nEvents :
        nEvents + m_eventNumberOffset[m_numberOfRuns-1]);
    file.Close();
    ++m_numberOfRuns;
  }
}
    
long DataDescription::numberOfEventsInRunFile(int i) const
{
  assert(i < m_numberOfRuns);
  return i == 0 ? m_eventNumberOffset[i] : m_eventNumberOffset[i] - m_eventNumberOffset[i-1];
}
