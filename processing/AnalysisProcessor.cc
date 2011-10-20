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
#include "LikelihoodAnalysis.hh"

AnalysisProcessor::AnalysisProcessor()
  : EventProcessor()
  , m_particle(new Particle)
  , m_particleFilter(new ParticleFilter)
  , m_cutFilter(new CutFilter)
  , m_mcFilter(new MCFilter)
  , m_trackFinding(new TrackFinding)
  , m_corrections(new Corrections)
  , m_identifier(new ParticleIdentifier)
  , m_likelihood(new LikelihoodAnalysis(Enums::TrackerMomentumLikelihood | Enums::TimeOfFlightLikelihood))
{
}

AnalysisProcessor::AnalysisProcessor(QVector<EventDestination*> destinations, Enums::TrackType track, Enums::Corrections flags)
  : EventProcessor(destinations)
  , m_particle(new Particle)
  , m_particleFilter(new ParticleFilter)
  , m_cutFilter(new CutFilter)
  , m_mcFilter(new MCFilter)
  , m_trackFinding(new TrackFinding)
  , m_corrections(new Corrections(flags))
  , m_identifier(new ParticleIdentifier)
  , m_likelihood(new LikelihoodAnalysis(Enums::TrackerMomentumLikelihood))
{
  setTrackType(track);
  setCorrectionFlags(flags);
}

AnalysisProcessor::~AnalysisProcessor()
{
  delete m_particle;
  delete m_particleFilter;
  delete m_mcFilter;
  delete m_cutFilter;
  delete m_trackFinding;
  delete m_corrections;
  delete m_identifier;
  delete m_likelihood;
}

void AnalysisProcessor::setTrackType(const Enums::TrackType& trackType)
{
  m_particle->setTrackType(trackType);
}

void AnalysisProcessor::setCorrectionFlags(const Enums::Corrections& flags)
{
  m_corrections->setFlags(flags);
}

void AnalysisProcessor::setParticleFilter(const Enums::Particles& types)
{
  m_particleFilter->setTypes(types);
}

void AnalysisProcessor::setMCFilter(const Enums::Particles& types)
{
  m_mcFilter->setTypes(types);
}

void AnalysisProcessor::setCutFilter(const CutFilter& cuts)
{
  m_cutFilter->setCuts(cuts);
}

void AnalysisProcessor::process(SimpleEvent* simpleEvent)
{
  m_particle->reset();
  m_corrections->preFitCorrections(simpleEvent);

  AnalyzedEvent* analyzedEvent = new AnalyzedEvent;
  analyzedEvent->setClusters(QVector<Hit*>::fromStdVector(simpleEvent->hits()));
  analyzedEvent->setParticle(m_particle);
  analyzedEvent->setSimpleEvent(simpleEvent);

  //check filters which need no reconstruction
  if (!m_mcFilter->passes(simpleEvent) || !m_cutFilter->passes(simpleEvent))
    return;

  QVector<Hit*> trackClusters = m_trackFinding->findTrack(analyzedEvent->clusters());

  Track* track = m_particle->track();
  TimeOfFlight* tof = m_particle->timeOfFlight();
  TRDReconstruction* trd = m_particle->trdReconstruction();
  ParticleInformation* info = m_particle->information();

  if (track) {
    track->fit(trackClusters);
    m_corrections->postFitCorrections(m_particle);
    tof->calculateTimes(track);
    m_corrections->postTOFCorrections(m_particle);
    trd->reconstructTRD(simpleEvent, track);
    info->process();
  }

  // identify particle species
  m_identifier->identify(m_particle);
  m_likelihood->identify(m_particle);

  if (m_particleFilter->passes(m_particle) && m_cutFilter->passes(analyzedEvent->clusters(), m_particle)) {
    QVector<EventDestination*> postponed;
    foreach(EventDestination* destination, m_destinations) {
      bool success = tryProcessingDestination(destination, analyzedEvent);
      if (!success)
        postponed.append(destination); // postpone this destination for now.
    }
    EventDestination** postponedData = postponed.data();
    unsigned int nPostponed = postponed.size();
    unsigned int i = 0;
    while (i < nPostponed) {
      if (tryProcessingDestination(postponedData[i], analyzedEvent))
        i++;
      else
        usleep(1000); // do not hammer the mutex locker with tryLock calls, better to sleep a ms
    }
  }
}
