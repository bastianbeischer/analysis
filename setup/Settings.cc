#include "Settings.hh"

Settings::Settings() :
  m_situation(Unknown),
  m_identifier(0),
  m_name(""),
  m_firstRun(0),
  m_lastRun(0),
  m_momentum(0),
  m_magnet(false)
{
}

Settings::~Settings()
{
}
