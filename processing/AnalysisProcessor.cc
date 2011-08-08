#include "AnalysisProcessor.hh"

#include "TrackFinding.hh"
#include "Cluster.hh"
#include "Hit.hh"
#include "SimpleEvent.hh"
#include "Setup.hh"
#include "EventDestination.hh"
#include "Particle.hh"
#include "ParticleFilter.hh"
#include "MCFilter.hh"
#include "ParticleIdentifier.hh"
#include "TimeOfFlight.hh"
#include "TRDReconstruction.hh"
#include "ParticleInformation.hh"

AnalysisProcessor::AnalysisProcessor()
  : EventProcessor()
  , m_particle(new Particle)
  , m_filter(new ParticleFilter)
  , m_mcFilter(new MCFilter)
  , m_trackFinding(new TrackFinding)
  , m_corrections(new Corrections)
  , m_identifier(new ParticleIdentifier)
  , m_cuts(new CutFilter)
{
}

AnalysisProcessor::AnalysisProcessor(QVector<EventDestination*> destinations, Track::Type track, Corrections::Flags flags)
  : EventProcessor(destinations)
  , m_particle(new Particle)
  , m_filter(new ParticleFilter)
  , m_mcFilter(new MCFilter)
  , m_trackFinding(new TrackFinding)
  , m_corrections(new Corrections(flags))
  , m_identifier(new ParticleIdentifier)
{
  setTrackType(track);
  setCorrectionFlags(flags);
}

AnalysisProcessor::~AnalysisProcessor()
{
  delete m_particle;
  delete m_filter;
  delete m_mcFilter;
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

void AnalysisProcessor::setParticleFilter(ParticleFilter::Types types)
{
  m_filter->setTypes(types);
}

void AnalysisProcessor::setMCFilter(MCFilter::Types types)
{
  m_mcFilter->setTypes(types);
}

void AnalysisProcessor::setCutFilter(CutFilter cuts)
{
  m_cuts->setCuts(cuts);
}

void AnalysisProcessor::process(SimpleEvent* event)
{
  m_particle->reset();

  m_corrections->preFitCorrections(event);

  QVector<Hit*> clusters = QVector<Hit*>::fromStdVector(event->hits());
  QVector<Hit*> trackClusters = m_trackFinding->findTrack(clusters);

  Track* track = m_particle->track();
  TimeOfFlight* tof = m_particle->timeOfFlight();
  TRDReconstruction* trd = m_particle->trdReconstruction();
  ParticleInformation* info = m_particle->information();

  if (track) {
    track->fit(trackClusters);
    m_corrections->postFitCorrections(m_particle);
    tof->calculateTimes(track);
    m_corrections->postTOFCorrections(m_particle);
    trd->reconstructTRD(event, track);
    info->process();
  }

  // identify particle species
  m_identifier->identify(m_particle);

  if (m_filter->passes(m_particle)) {
    if (m_mcFilter->passes(clusters, m_particle, event)) {
      if (m_cuts->passes(clusters, m_particle, event)) {
        QVector<EventDestination*> postponed;
        foreach(EventDestination* destination, m_destinations) {
          bool success = tryProcessingDestination(destination, clusters, m_particle, event);
          if (!success)
            postponed.append(destination); // postpone this destination for now.
        }
        while(postponed.size() > 0) {
          bool success = tryProcessingDestination(postponed.front(), clusters, m_particle, event);
          if (success)
            postponed.remove(0);
        }
      }
    }
  }
}
