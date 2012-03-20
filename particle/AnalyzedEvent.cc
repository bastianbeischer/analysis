#include "AnalyzedEvent.hh"

#include "Track.hh"
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

bool AnalyzedEvent::hasTrack() const
{
  Q_ASSERT(m_particle);
  return m_particle->track();
}

bool AnalyzedEvent::hasGoodTrack() const
{
  Q_ASSERT(m_particle);
  Track* track = m_particle->track();
  return track && track->fitGood();
}

Track* AnalyzedEvent::goodTrack() const
{
  return hasGoodTrack() ? m_particle->track() : 0;
}

const Settings* AnalyzedEvent::settings() const
{
  return SettingsManager::instance()->settingsForEvent(m_simpleEvent);
}

bool AnalyzedEvent::flagsMatch(Enums::EventFlags flags) const
{
  Q_ASSERT(m_particle);
  Q_ASSERT(m_particle->information());
  return m_particle->information()->eventFlags() == flags;
}

bool AnalyzedEvent::flagsSet(Enums::EventFlags flags) const
{
  Q_ASSERT(m_particle);
  Q_ASSERT(m_particle->information());
  return ((m_particle->information()->eventFlags() & flags) == flags) && (flags != 0);
}
