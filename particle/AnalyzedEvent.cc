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
}

void AnalyzedEvent::reset()
{
  m_clusters.clear();
  m_particle = 0;
  m_simpleEvent = 0;
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
