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
#include "AnalysisProcessorSetting.hh"
#include "Helpers.hh"

#include <QDebug>

AnalysisProcessor::AnalysisProcessor()
  : EventProcessor()
  , m_particles(Enums::NoParticle)
  , m_likelihoods(Enums::UndefinedLikelihood)
  , m_analyzedEvent(new AnalyzedEvent)
  , m_particle(new Particle)
  , m_particleFilter(new ParticleFilter)
  , m_cutFilter(new CutFilter)
  , m_mcFilter(new MCFilter)
  , m_trackFinding(new TrackFinding)
  , m_corrections(new Corrections)
  , m_identifier(new ParticleIdentifier)
  , m_reconstructions()
{
  AnalysisProcessorSetting setting;
  setting.load(Helpers::analysisPath() + "/conf/defaultAnalysisSetting.conf");
  setAnalysisProcessorSetting(setting);
}

AnalysisProcessor::AnalysisProcessor(QVector<EventDestination*> destinations, const AnalysisProcessorSetting& setting)
  : EventProcessor(destinations)
  , m_particles(Enums::NoParticle)
  , m_likelihoods(Enums::UndefinedLikelihood)
  , m_analyzedEvent(new AnalyzedEvent)
  , m_particle(new Particle)
  , m_particleFilter(new ParticleFilter)
  , m_cutFilter(new CutFilter)
  , m_mcFilter(new MCFilter)
  , m_trackFinding(new TrackFinding)
  , m_corrections(new Corrections)
  , m_identifier(new ParticleIdentifier)
  , m_reconstructions()
{
  setAnalysisProcessorSetting(setting);
}

void AnalysisProcessor::setAnalysisProcessorSetting(const AnalysisProcessorSetting& setting)
{
  m_particles = setting.particles();
  m_likelihoods = setting.likelihoods();
  setTrackType(setting.trackType());
  setCorrectionFlags(setting.corrections());
  setReconstructionMethod(setting.reconstructionMethod());
  setParticleFilter(setting.particleFilter());
  setMCFilter(setting.mcParticleFilter());
  setCutFilter(setting.cutFilter());

  Enums::LikelihoodVariableIterator end = Enums::likelihoodVariableEnd();

  Enums::LikelihoodVariables internal = Enums::UndefinedLikelihood;
  Enums::LikelihoodVariables external = Enums::UndefinedLikelihood;
  for (Enums::LikelihoodVariableIterator it = Enums::likelihoodVariableBegin(); it != end; ++it) {
    if (it.key() & m_likelihoods) {
      if (Enums::isInternalLikelihoodVariable(it.key())) {
        internal|= it.key();
      } else {
        external|= it.key();
      }
    }
  }

  Enums::ReconstructionMethods methods = setting.reconstructionMethods() | setting.reconstructionMethod();
  for (Enums::ReconstructionMethodIterator it = Enums::reconstructionMethodBegin(); it != Enums::reconstructionMethodEnd(); ++it) {
    if (it.key() & methods) {
      bool isInternal = Enums::isInternalReconstructionMethod(it.key());
      Enums::LikelihoodVariables variables = isInternal ? internal : (internal | external);
      Reconstruction* method = Reconstruction::newReconstruction(it.key(), variables, m_particles);
      m_reconstructions.insert(it.key(), method);
    }
  }
}

AnalysisProcessor::~AnalysisProcessor()
{
  delete m_analyzedEvent;
  delete m_particle;
  delete m_particleFilter;
  delete m_mcFilter;
  delete m_cutFilter;
  delete m_trackFinding;
  delete m_corrections;
  delete m_identifier;
  qDeleteAll(m_reconstructions);
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

  m_analyzedEvent->reset();
  m_analyzedEvent->setClusters(QVector<Hit*>::fromStdVector(simpleEvent->hits()));
  m_analyzedEvent->setParticle(m_particle);
  m_analyzedEvent->setSimpleEvent(simpleEvent);

  //check filters which need no reconstruction
  if (!m_mcFilter->passes(simpleEvent) || !m_cutFilter->passes(simpleEvent))
    return;

  QVector<Hit*> trackClusters = m_trackFinding->findTrack(m_analyzedEvent->clusters());

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
    (*it)->identify(m_analyzedEvent);

  if (m_particleFilter->passes(m_particle) && m_cutFilter->passes(m_analyzedEvent)) {
    QVector<EventDestination*> postponed;
    foreach(EventDestination* destination, m_destinations) {
      if (destination->eventFlagFilter().passes(m_analyzedEvent)) {
        bool success = tryProcessingDestination(destination, m_analyzedEvent);
        if (!success)
          postponed.append(destination); // postpone this destination for now.
      }
    }
    EventDestination** postponedData = postponed.data();
    unsigned int nPostponed = postponed.size();
    unsigned int i = 0;
    while (i < nPostponed) {
      if (tryProcessingDestination(postponedData[i], m_analyzedEvent)) {
        i++;
      } else {
        usleep(1000); // do not hammer the mutex locker with tryLock calls, better to sleep a ms
      }
    }
  }
}

const Reconstruction* AnalysisProcessor::reconstruction(Enums::ReconstructionMethod method) const
{
  QMap<Enums::ReconstructionMethod, Reconstruction*>::ConstIterator it = m_reconstructions.find(method);
  return it == m_reconstructions.end() ? 0 : *it;
}
