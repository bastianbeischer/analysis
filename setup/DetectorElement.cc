#include "DetectorElement.hh"

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
