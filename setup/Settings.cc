#include "Settings.hh"

#include <cmath>

Settings::Settings() :
  m_situation(Enums::NoSituation),
  m_identifier(0),
  m_name(""),
  m_firstRun(0),
  m_lastRun(0),
  m_momentum(0),
  m_polarity(0),
  m_pressure(0.),
  m_magnet(false)
{
}

Settings::~Settings()
{
}

bool Settings::isAboveThreshold(const double& mass) const
{
  double dndp = 4.51819e-4;
  double energy = sqrt(m_momentum*m_momentum + mass*mass);
  double threshold = (energy/m_momentum - 1)/dndp;
  return m_pressure > threshold;
}
