#include "AnalysisProcessor.hh"

#include "TrackFinding.hh"
#include "Cluster.hh"
#include "Hit.hh"
#include "SimpleEvent.hh"
#include "Setup.hh"
#include "EventDestination.hh"
#include "Particle.hh"
#include "ParticleIdentifier.hh"
#include "TimeOfFlight.hh"
#include "ParticleInformation.hh"

AnalysisProcessor::AnalysisProcessor()
  : EventProcessor()
  , m_particle(new Particle)
  , m_trackFinding(new TrackFinding)
  , m_corrections(new Corrections)
  , m_identifier(new ParticleIdentifier)
{
}

AnalysisProcessor::AnalysisProcessor(QVector<EventDestination*> destinations, Track::Type track, Corrections::Flags flags)
  : EventProcessor(destinations)
  , m_particle(new Particle)
  , m_trackFinding(new TrackFinding)
  , m_corrections(new Corrections(flags))
  , m_identifier(new ParticleIdentifier)
{
  setTrackType(track);
  setCorrectionFlags(flags);
}

AnalysisProcessor::~AnalysisProcessor()
{
  delete m_trackFinding;
  delete m_corrections;
  delete m_identifier;
}

void AnalysisProcessor::setTrackType(Track::Type trackType)
{
  m_particle->setTrackType(trackType);
}

void AnalysisProcessor::setCorrectionFlags(Corrections::Flags flags)
{
  m_corrections->setFlags(flags);
}

void AnalysisProcessor::process(SimpleEvent* event)
{
  m_corrections->preFitCorrections(event);

  QVector<Hit*> clusters = QVector<Hit*>::fromStdVector(event->hits());
  QVector<Hit*> trackClusters = m_trackFinding->findTrack(clusters);

  Track* track = m_particle->track();
  TimeOfFlight* tof = m_particle->timeOfFlight();
  ParticleInformation* info = m_particle->information();

  if (track) {
    info->reset();
    track->fit(trackClusters);
    m_corrections->postFitCorrections(track);
    tof->calculateTimes(track);
    info->process();
  }

  // identify particle species
  m_identifier->identify(m_particle);

  foreach (EventDestination* destination, m_destinations)
    destination->processEvent(clusters, m_particle, event);
}
