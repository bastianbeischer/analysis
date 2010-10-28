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
}

DataDescription::~DataDescription()
{}

void DataDescription::calculateSoftwareVersionHash()
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

    
void DataDescription::addRunFile(const std::string& fileName, const int nEvents)
{
  m_runFileNames.push_back(fileName);
  m_eventNumberOffset.push_back(m_numberOfRuns == 0 ?
                                nEvents :
                                nEvents + m_eventNumberOffset[m_numberOfRuns-1]);
}
    
long DataDescription::numberOfEventsInRunFile(int i) const
{
  assert(i < m_numberOfRuns);
  return i == 0 ? m_eventNumberOffset[i] : m_eventNumberOffset[i] - m_eventNumberOffset[i-1];
}
