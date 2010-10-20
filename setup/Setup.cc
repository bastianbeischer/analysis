#include "Setup.hh"

#include "DetectorElements.hh"

Setup* Setup::m_instance = 0;

Setup::Setup()
{
}

Setup::~Setup()
{
}

Setup* Setup::instance()
{
  if (!m_instance) m_instance = new Setup;
  return m_instance;
}

DetectorElement* Setup::element(unsigned short id)
{
  if (!m_elements[id]) m_elements[id] = new DetectorElement;
  return m_elements[id];
}
