#include "TOFSipmHit.hh"

#include <assert.h>
#include <algorithm>

#include <iostream>

ClassImp( TOFSipmHit );

TOFSipmHit::TOFSipmHit()
  : Hit()
  , m_startTime(0)
  , m_timeOverThreshold(0)
  , m_levelChanges()
{}

TOFSipmHit::TOFSipmHit(unsigned short detId, TVector3 position, TVector3 counterPosition)
  : Hit(Hit::tof, detId, 0, position, counterPosition)
  , m_startTime(0)
  , m_timeOverThreshold(0)
  , m_levelChanges()
{}


TOFSipmHit::~TOFSipmHit()
{}

void TOFSipmHit::processTDCHits()
{
  std::vector<double> tot;
  std::vector<double> start;
  for (int i = 0; i < numberOfLevelChanges() - 1; ++i) {
    if (edgeType(i) == LeadingEdge && edgeType(i+1) == TrailingEdge) {
      tot.push_back(time(i+1) - time(i));
      start.push_back(time(i));
    }
  }
  if (tot.size() > 0) {
    double maxTot = tot[0];
    double startBelongingToMaxTot = start[0];
    for (unsigned int i = 1; i < tot.size(); ++i) {
      if (maxTot < tot[i] && maxTot < 70.) {
        maxTot = tot[i];
        startBelongingToMaxTot = start[i];
      }
    }
    m_timeOverThreshold = maxTot;
    m_signalHeight = 10 * maxTot;
    m_startTime = startBelongingToMaxTot;
    return;
  }
  m_timeOverThreshold = 0;
  m_signalHeight = 0;
  m_startTime = 0;
}

double TOFSipmHit::time(int i) const
{
  assert(0 <= i && i < numberOfLevelChanges());
  return timeFromData(m_levelChanges[i]);
}

TOFSipmHit::EdgeType TOFSipmHit::edgeType(int i) const
{
  assert(0 <= i && i < numberOfLevelChanges());
  return (m_levelChanges[i] & 0x02000000) ? TrailingEdge: LeadingEdge;
}

int TOFSipmHit::channel() const
{
  assert(numberOfLevelChanges() != 0);
  return channelFromData(m_levelChanges[0]);
}

int TOFSipmHit::spillNumber(int i) const
{
  assert(0 <= i && i < numberOfLevelChanges());
  return (m_levelChanges[i] >> 26) & 0x3F;
}

int TOFSipmHit::channelFromData(uint32_t value)
{
  return (value & 0x01F80000) >> 19;
}

double TOFSipmHit::timeFromData(uint32_t value)
{
  return .1 * (value & 0x0007FFFF);
}

bool TOFSipmHit::earlierThan(uint32_t data1, uint32_t data2)
{
  if (channelFromData(data1) < channelFromData(data2))
    return true;
  if (timeFromData(data1) < timeFromData(data2))
    return true;
  return false;
}

void TOFSipmHit::setLevelChange(int i, uint32_t value)
{
  assert(0 <= i && i < numberOfLevelChanges());
  m_levelChanges[i] = value;
  std::sort(m_levelChanges.begin(), m_levelChanges.end(), earlierThan);
}

void TOFSipmHit::addLevelChange(uint32_t value)
{
  m_levelChanges.push_back(0);
  setLevelChange(numberOfLevelChanges() - 1, value);
}

void TOFSipmHit::dump()
{
  std::cout
    << "startTime()=" << startTime()
    << " timeOverThreshold()=" << timeOverThreshold()
    << " numberOfLevelChanges()=" << numberOfLevelChanges()
    << " channel()=" << channel() << std::endl;
  for (int i = 0; i < numberOfLevelChanges(); ++i) {
    std::cout
      << time(i)
      << (edgeType(i) == LeadingEdge ? 'L' : 'T')
      << ' ';
  }
  std::cout << std::endl << std::flush;
}
