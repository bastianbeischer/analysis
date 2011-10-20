#include "AnalyzedEvent.hh"

#include "Hit.hh"
#include "SimpleEvent.hh"
#include "Particle.hh"
#include "Settings.hh"
#include "SettingsManager.hh"

AnalyzedEvent::AnalyzedEvent()
  : m_clusters()
  , m_particle(0)
  , m_simpleEvent(0)
{
}

AnalyzedEvent::~AnalyzedEvent()
{
  for (QVector<Hit*>::Iterator it = m_clusters.begin(); it != m_clusters.end(); ++it)
    delete *it;
  if (m_particle)
    delete m_particle;
  if (m_simpleEvent)
    delete m_simpleEvent;
}

const Settings* AnalyzedEvent::settings() const
{
  return SettingsManager::instance()->settingsForEvent(m_simpleEvent);
}

bool AnalyzedEvent::flagsMatch(ParticleInformation::Flags flags)
{
  Q_ASSERT(m_particle);
  Q_ASSERT(m_particle->information());
  return m_particle->information()->flags() == flags;
}

bool AnalyzedEvent::flagsSet(ParticleInformation::Flags flags)
{
  Q_ASSERT(m_particle);
  Q_ASSERT(m_particle->information());
  return ((m_particle->information()->flags() & flags) == flags) && (flags != 0);
}
