#include "Setup.hh"

#include "DetectorElement.hh"
#include "SiPMArray.hh"

Setup* Setup::m_instance = 0;

Setup::Setup()
{
}

Setup::~Setup()
{
  foreach(DetectorElement* element, m_elements)
    delete element;
}

Setup* Setup::instance()
{
  if (!m_instance) m_instance = new Setup;
  return m_instance;
}

// this should differentiate between types of detector elements soon
DetectorElement* Setup::element(unsigned short id)
{
  if (!m_elements[id]) m_elements[id] = new SiPMArray;
  return m_elements[id];
}
