#include "DataDescription.hh"
#include "SimpleEvent.hh"

#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <iostream>
#include <sstream>
#include <cstdlib>

#include <TFile.h>
#include <TTree.h>

ClassImp(DataDescription);

DataDescription::DataDescription()
  : TObject()
  , m_numberOfRuns(0)
{
}

DataDescription::DataDescription(const DataDescription& other)
  : TObject(other)
  , m_comment(other.m_comment)
  , m_softwareVersionHash(other.m_softwareVersionHash)
  , m_numberOfEvents(other.m_numberOfEvents)
  , m_numberOfCalibrationEvents(other.m_numberOfCalibrationEvents)
  , m_runFileNames(other.m_runFileNames)
  , m_runFileSoftwareVersionHash(other.m_runFileSoftwareVersionHash)
  , m_numberOfRuns(other.m_numberOfRuns)
{
}

DataDescription::~DataDescription()
{}

const std::string DataDescription::calculateSoftwareVersionHash()
{
  FILE* file = popen("cd $PERDAIXANA_PATH && git rev-parse HEAD | tr -d '\n'", "r");
  char line[41];
  if (fscanf(file, "%s", line) != 1)
    std::cerr << "Could not read SHA1 hash!" << std::endl;
  pclose(file);
  return line;
}

void DataDescription::setSoftwareVersionHash()
{
  m_softwareVersionHash = calculateSoftwareVersionHash();
  assert(m_softwareVersionHash.length() == 40);
}

void DataDescription::addRunFile(const std::string& fileName, const std::string& softwareVersionHash, const int nEvents, const int nCalibrationEvents)
{
  m_runFileNames.push_back(fileName);
  m_numberOfEvents.push_back(nEvents);
  m_numberOfCalibrationEvents.push_back(nCalibrationEvents);
  m_runFileSoftwareVersionHash.push_back(softwareVersionHash);
  ++m_numberOfRuns;
}

long DataDescription::eventNumberInRunFile(long eventNumber) const
{
  assert(m_numberOfRuns > 0);
  int runNo = runFileForEventNumber(eventNumber);
  long entryInFile = eventNumber;
  for (int i = 0; i < runNo; ++i) {
    entryInFile -= m_numberOfEvents[i];
  }
  assert(entryInFile >= 0);
  return entryInFile;
}

int DataDescription::runFileForEventId(long eventId) const
{
  assert(m_numberOfRuns > 0);
  int runNo = 0;
  long totalEvents = m_numberOfEvents[0] + m_numberOfCalibrationEvents[0];
  while (eventId >= totalEvents) {
    ++runNo;
    if (runNo == m_numberOfRuns)
      break;
    totalEvents += m_numberOfEvents[runNo] + m_numberOfCalibrationEvents[runNo];
  }
  assert(runNo < m_numberOfRuns);
  return runNo;
}

int DataDescription::runFileForEventNumber(long eventNumber) const
{
  assert(m_numberOfRuns > 0);
  int runNo = 0;
  long totalEvents = m_numberOfEvents[0];
  while (eventNumber >= totalEvents) {
    ++runNo;
    if (runNo == m_numberOfRuns)
      break;
    totalEvents += m_numberOfEvents[runNo];
  }
  assert(runNo < m_numberOfRuns);
  return runNo;
}

const std::string& DataDescription::runFileNameForEventNumber(long eventNumber) const
{
  int runNo = runFileForEventNumber(eventNumber);
  return m_runFileNames[runNo];
}

long DataDescription::numberOfEventsInRunFile(int i) const
{
  assert(i < m_numberOfRuns);
  return m_numberOfEvents[i];
}

long DataDescription::numberOfCalibrationEventsInRunFile(int i) const
{
  assert(i < m_numberOfRuns);
  return m_numberOfCalibrationEvents[i];
}
