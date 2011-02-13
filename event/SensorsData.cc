#include "SensorsData.hh"

#include <TFile.h>
#include <TTree.h>

#include <cmath>
#include <iostream>

SensorsData::SensorsData(DataType type, const char* filename) :
  m_type(type),
  m_file(0),
  m_tree(0),
  m_firstTime(0),
  m_good(false)
{
  m_good = addRootFile(filename);
}

SensorsData::~SensorsData()
{
  delete m_file;
}

bool SensorsData::addRootFile(const char* filename)
{
  TFile* m_file = new TFile(filename);
  if (m_file->IsZombie()) {
    delete m_file;
    m_file = 0;
    return false;
  } 

  switch (m_type) {
  case SENSORS:
    m_tree = static_cast<TTree*>(m_file->Get("sensors"));
    break;
  case ATC:
    m_tree = static_cast<TTree*>(m_file->Get("ATC"));
    break;
  case EBASS:
    m_tree = static_cast<TTree*>(m_file->Get("EBASS"));
    break;
  }

  if (m_tree == 0) {
    return false;
  }

  unsigned int time;
  m_tree->SetBranchAddress("time",&time);
  m_tree->GetEntry(0);
  m_firstTime = time;

  return true;
}

int SensorsData::entryForTime(unsigned int time) const
{
  if (time < m_firstTime || time > m_firstTime + m_tree->GetEntries()) {
    std::cerr << "Tree does not contain the requested time " << time 
              << " (first time = " << m_firstTime << ", entries = " << m_tree->GetEntries() << ")" << std::endl;
    return 0;
  }

  return time - m_firstTime;
}


float SensorsData::previousValue(const char* id, unsigned int time, int& diff)
{
  if (!m_tree->GetBranch(id)) {
    std::cerr << "Branch " << id << "not found.";
    return sqrt(-1);
  }

  int exactEntry = entryForTime(time);
  
  float var = sqrt(-1);
  m_tree->SetBranchAddress(id,&var);
  m_tree->GetEntry(exactEntry);
  int entry = exactEntry;
  while(!(var == var) && entry >= 0 && entry < m_tree->GetEntries()) {
    m_tree->GetEntry(entry);
    entry--;
  }
  diff = entry - exactEntry;

  return var;
}

float SensorsData::nextValue(const char* id, unsigned int time, int& diff)
{
  if (!m_tree->GetBranch(id)) {
    std::cerr << "Branch " << id << "not found.";
    return sqrt(-1);
  }

  if (time < m_firstTime || time > m_firstTime + m_tree->GetEntries()) {
    std::cerr << "Tree does not contain the requested time " << time 
              << " (first time = " << m_firstTime << ", entries = " << m_tree->GetEntries() << ")" << std::endl;
    return sqrt(-1);
  }

  int exactEntry = entryForTime(time);

  float var = sqrt(-1);
  m_tree->SetBranchAddress(id,&var);
  m_tree->GetEntry(exactEntry);
  int entry = exactEntry;
  while(!(var == var) && entry >= 0 && entry < m_tree->GetEntries()) {
    m_tree->GetEntry(entry);
    entry++;
  }
  diff = entry - exactEntry;

  return var;
}

float SensorsData::averageValue(const char* id, unsigned int time)
{
  // int prevDiff = 0;
  // float prevValue = previousValue(id, time, prevDiff);
  // int nextDiff = 0;
  // float nextValue = nextValue(id, time, nextDiff);

  // float distance = fabs(nextDiff - prevDiff);
  return 0.;
}

char** SensorsData::keys() const
{
  TObjArray* branches = m_tree->GetListOfBranches();
  char** keys = new char*[branches->GetEntries()];
  
  int i = 0;
  int k = 0;
  while (k < branches->GetEntries()) {
    if (strcmp(branches->At(i)->GetName(),"time") != 0) {
      keys[i] = const_cast<char*>(branches->At(k)->GetName());
      i++;
    }
    k++;
  }
  return keys;
}

int SensorsData::numberOfKeys() const
{
  TObjArray* branches = m_tree->GetListOfBranches();
  return branches->GetEntries()-1;
}
