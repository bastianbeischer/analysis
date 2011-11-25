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
#include "Reconstruction.hh"

#include <QDebug>

AnalysisProcessor::AnalysisProcessor()
  : EventProcessor()
  , m_particle(new Particle)
  , m_particles(Setup::instance()->proposedParticles())
  , m_particleFilter(new ParticleFilter)
  , m_cutFilter(new CutFilter)
  , m_mcFilter(new MCFilter)
  , m_trackFinding(new TrackFinding)
  , m_corrections(new Corrections)
  , m_identifier(new ParticleIdentifier)
  , m_reconstructions()
{
  initReconstructions();
}

AnalysisProcessor::AnalysisProcessor(QVector<EventDestination*> destinations, Enums::TrackType track, Enums::Corrections flags, Enums::Particles particles)
  : EventProcessor(destinations)
  , m_particle(new Particle)
  , m_particles(particles)
  , m_particleFilter(new ParticleFilter)
  , m_cutFilter(new CutFilter)
  , m_mcFilter(new MCFilter)
  , m_trackFinding(new TrackFinding)
  , m_corrections(new Corrections(flags))
  , m_identifier(new ParticleIdentifier)
  , m_reconstructions()
{
  setTrackType(track);
  setCorrectionFlags(flags);
  initReconstructions();
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
  qDeleteAll(m_reconstructions);
}

void AnalysisProcessor::initReconstructions()
{
  Q_ASSERT(m_reconstructions.count() == 0); // should only be called once!

  Enums::LikelihoodVariables internal = Enums::TrackerMomentumLikelihood | Enums::TimeOfFlightLikelihood
    | Enums::SignalHeightTrdLikelihood;
  QVector<Enums::ReconstructionMethod> internalReconstruction = QVector<Enums::ReconstructionMethod>()
    << Enums::Spectrometer << Enums::TOF << Enums::WeightedMean << Enums::Chi2 << Enums::Likelihood;
  foreach (Enums::ReconstructionMethod method, internalReconstruction)
    m_reconstructions.insert(method, Reconstruction::newReconstruction(method, internal, m_particles));

  // include e.g. Cherenkov counters
  Enums::LikelihoodVariables external = Enums::CherenkovLikelihood;
  QVector<Enums::ReconstructionMethod> externalReconstruction = QVector<Enums::ReconstructionMethod>()
    << Enums::SpectrometerExternalInformation << Enums::TOFExternalInformation << Enums::WeightedMeanExternalInformation
    << Enums::Chi2ExternalInformation << Enums::LikelihoodExternalInformation;
  foreach (Enums::ReconstructionMethod method, externalReconstruction)
    m_reconstructions.insert(method, Reconstruction::newReconstruction(method, internal | external, m_particles));
}

void AnalysisProcessor::setTrackType(Enums::TrackType trackType)
{
  m_particle->setTrackType(trackType);
}

void AnalysisProcessor::setReconstructionMethod(Enums::ReconstructionMethod method)
{
  m_particle->setReconstructionMethod(method);
}

void AnalysisProcessor::setCorrectionFlags(Enums::Corrections flags)
{
  m_corrections->setFlags(flags);
}

void AnalysisProcessor::setParticleFilter(Enums::Particles types)
{
  m_particleFilter->setTypes(types);
}

void AnalysisProcessor::setMCFilter(Enums::Particles types)
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
  QMap<Enums::ReconstructionMethod, Reconstruction*>::Iterator end = m_reconstructions.end();
  for (QMap<Enums::ReconstructionMethod, Reconstruction*>::Iterator it = m_reconstructions.begin(); it != end; ++it)
    (*it)->identify(analyzedEvent);

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

const Reconstruction* AnalysisProcessor::reconstruction(Enums::ReconstructionMethod method) const
{
  const Reconstruction* reconstruction = m_reconstructions[method];
  Q_ASSERT(reconstruction);
  return reconstruction;
}
