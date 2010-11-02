#include "DetectorElement.hh"

#include <QtAlgorithms>

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
  qSort(m_hits.begin(), m_hits.end(), comparePositions);
}
