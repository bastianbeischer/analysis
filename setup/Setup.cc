#include "Setup.hh"

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
