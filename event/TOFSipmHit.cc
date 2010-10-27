#include "TOFSipmHit.hh"

#include <assert.h>
#include <algorithm>

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
  //TODO: Use TOT to get the wanted signal pusle.
  m_startTime = time(0);
  //TODO: What if there is a noise pulse?
  m_timeOverThreshold = time(1) - time(0);
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

int TOFSipmHit::channel(int i) const
{
  assert(0 <= i && i < numberOfLevelChanges());
  return channelFromData(m_levelChanges[i]);
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
  return 10. * (value & 0x0007FFFF);
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
