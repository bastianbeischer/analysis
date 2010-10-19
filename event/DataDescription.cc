#include "DataDescription.hh"

#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <iostream>
#include <sstream>

ClassImp(DataDescription);

DataDescription::DataDescription()
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

long DataDescription::numberOfRuns() const
{
  //TODO
  return 0;
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

