#include "SensorsData.hh"

#include <TFile.h>
#include <TTree.h>

#include <cmath>
#include <cassert>
#include <iostream>

SensorsData::SensorsData(DataType type, const char* filename) :
  m_type(type),
  m_file(0),
  m_tree(0),
  m_nKeys(0),
  m_keys(0),
  m_firstTime(0),
  m_good(false),
  m_time(0),
  m_var(sqrt(-1)),
  m_values(0)
{
  m_good = addRootFile(filename);
  if (m_good) init();
}

SensorsData::~SensorsData()
{
  delete m_file;
  delete [] m_keys;
  delete [] m_values;
}

bool SensorsData::addRootFile(const char* filename)
{
  m_file = new TFile(filename);
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

  return true;
}

int SensorsData::entryForTime(unsigned int time) const
{
  if (time < m_firstTime || time > m_firstTime + m_tree->GetEntries()) {
    // std::cerr << "Tree does not contain the requested time " << time 
    //           << " (first time = " << m_firstTime << ", entries = " << m_tree->GetEntries() << ")" << std::endl;
    return -1;
  }

  return time - m_firstTime;
}


float SensorsData::previousValue(const char* id, unsigned int time, int& diff)
{
  if (!m_tree->GetBranch(id)) {
    std::cerr << "Branch " << id << "not found.";
    return sqrt(-1);
  }

  long exactEntry = entryForTime(time);
  
  int N = m_tree->GetEntries();
  m_var = sqrt(-1);

  m_tree->SetBranchStatus(id, 1);
  m_tree->SetBranchAddress(id,&m_var);

  long entry = exactEntry;
  m_tree->GetEntry(entry);
  while(entry >= 0 && entry < N && std::isnan(m_var)) {
    m_tree->GetEntry(entry);
    entry--;
  }
  m_tree->SetBranchStatus(id, 0);

  if (entry == exactEntry)
    diff = 0;
  else
    diff = entry - exactEntry + 1;

  return m_var;
}

float SensorsData::nextValue(const char* id, unsigned int time, int& diff)
{
  if (!m_tree->GetBranch(id)) {
    std::cerr << "Branch " << id << "not found.";
    return sqrt(-1);
  }

  int exactEntry = entryForTime(time);

  int N = m_tree->GetEntries();
  m_var = sqrt(-1);

  m_tree->SetBranchStatus(id, 1);
  m_tree->SetBranchAddress(id,&m_var);

  long entry = exactEntry;
  m_tree->GetEntry();
  while(entry >= 0 && entry < N && std::isnan(m_var)) {
    m_tree->GetEntry(entry);
    entry++;
  }
  m_tree->SetBranchStatus(id, 0);

  if (entry == exactEntry)
    diff = 0;
  else
    diff = entry - exactEntry - 1;

  return m_var;
}

float SensorsData::averageValue(const char* id, unsigned int time)
{
  int prevDiff = 0;
  float prevVal = previousValue(id, time, prevDiff);
  int nextDiff = 0;
  float nextVal = nextValue(id, time, nextDiff);

  if (prevDiff == 0) {
    // by the way the algorithm works, nextDiff should now also be 0!
    assert(nextDiff == prevDiff);
    return prevVal;
  }

  float dy = nextVal - prevVal;
  int dx = fabs(nextDiff - prevDiff);

  float average = prevVal + fabs(prevDiff)*dy/dx;

  return average;
}

float* SensorsData::values(unsigned int time) const
{
  int entry = entryForTime(time);
  if (entry >= 0)
    m_tree->GetEntry(entry);
  else {
    for (int i = 0; i < m_nKeys; i++) {
      m_values[i] = sqrt(-1);
    }
  }

  return m_values;
}

void SensorsData::init()
{
  TObjArray* branches = m_tree->GetListOfBranches();
  m_nKeys = branches->GetEntries()-1;
  m_keys = new const char*[m_nKeys];
  m_values = new float[m_nKeys];

  int i = 0;
  for (int k = 0; k < branches->GetEntries(); ++k) {
    const char* key = const_cast<char*>(branches->At(k)->GetName());
    if (strcmp(key, "time") != 0) {
      m_keys[i] = key;
      m_tree->SetBranchAddress(m_keys[i], &m_values[i]);
      i++;
    }
  }

  m_tree->SetBranchAddress("time",&m_time);
  m_tree->GetEntry(0);
  m_firstTime = m_time;
}

