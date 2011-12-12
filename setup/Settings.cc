#include "Settings.hh"
#include "ParticleProperties.hh"

#include <cmath>

Settings::Settings()
  : m_situation(Enums::NoSituation)
  , m_identifier(0)
  , m_name("")
  , m_firstRun(0)
  , m_lastRun(0)
  , m_momentum(0)
  , m_polarity(0)
  , m_pressure(0.)
  , m_magnet(false)
{
}

Settings::~Settings()
{
}

Enums::Particles Settings::aboveThreshold(int* numberOfParticlesAboveThreshold) const
{
  if (numberOfParticlesAboveThreshold)
    *numberOfParticlesAboveThreshold = 0;
  Enums::Particles particles;
  Enums::ParticleIterator end = Enums::particleEnd();
  for (Enums::ParticleIterator it = Enums::particleBegin(); it != end; ++it) {
    if (it.key() == Enums::NoParticle)
      continue;
    ParticleProperties properties(it.key());
    if (properties.charge() != 0 && isAboveThreshold(properties.mass())) {
      if (numberOfParticlesAboveThreshold)
        ++(*numberOfParticlesAboveThreshold);
      particles|= it.key();
    }
  }
  return particles;
}

bool Settings::isAboveThreshold(double mass) const
{
  double dndp = 4.51819e-4;
  double energy = sqrt(m_momentum*m_momentum + mass*mass);
  double threshold = (energy/m_momentum - 1)/dndp;
  return m_pressure > threshold;
}
