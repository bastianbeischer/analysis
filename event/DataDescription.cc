#include "DataDescription.hh"

#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <iostream>
#include <sstream>

ClassImp(DataDescription);

DataDescription::DataDescription()
{
  FILE* file = popen("git rev-parse HEAD", "r");
  char line[128];
  std::stringstream stream;
  while (fgets(line, sizeof(line), file)) {
    stream << line;
  }
  pclose(file);
  m_softwareVersionHash = stream.str();
  assert(m_softwareVersionHash.length() == 41);
}

DataDescription::~DataDescription()
{}

