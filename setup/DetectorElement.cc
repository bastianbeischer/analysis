#include "DetectorElement.hh"

#include <QtAlgorithms>

#include <cassert>

bool comparePositions(const Hit* hit1, const Hit* hit2)
{
  return hit1->position().x() < hit2->position().x();
}

DetectorElement::DetectorElement() :
  m_type(none),
  m_id(0),
  m_alignmentShift(0.)
{
}

DetectorElement::DetectorElement(unsigned short id) :
  m_type(none),
  m_id(id),
  m_alignmentShift(0.)
{
}

DetectorElement::~DetectorElement()
{
}

void DetectorElement::sortHits()
{
  // check if all channels are present --> otherwise reinserting into map will be wrong if a channel was skipped
  if (m_type == tracker)
    assert(m_hits.size() == 32);
  else if (m_type == trd)
    assert(m_hits.size() == 16);
  // else if (m_type == tof)
  //   assert(hits.size() == 4);

  QList<Hit*> hits = m_hits.values();
  qSort(hits.begin(), hits.end(), comparePositions);
  for (int i = 0; i < hits.size(); i++) {
    m_hits[i] = hits.at(i);
  }
}
