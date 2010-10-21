#include "DetectorElement.hh"

DetectorElement::DetectorElement() :
  m_id(0)
{
}

DetectorElement::DetectorElement(unsigned short id) :
  m_id(id)
{
}

DetectorElement::~DetectorElement()
{
}
