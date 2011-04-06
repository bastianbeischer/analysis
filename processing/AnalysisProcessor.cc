#include "AnalysisProcessor.hh"

#include "TrackFinding.hh"
#include "CenteredBrokenLine.hh"
#include "CenteredBrokenLine2D.hh"
#include "BrokenLine.hh"
#include "StraightLine.hh"
#include "TrackFinding.hh"
#include "Cluster.hh"
#include "Hit.hh"
#include "SimpleEvent.hh"
#include "Setup.hh"
#include "EventDestination.hh"
#include "Particle.hh"

AnalysisProcessor::AnalysisProcessor()
  : EventProcessor()
  , m_particle(new Particle)
  , m_track(0)
  , m_trackFinding(new TrackFinding)
  , m_corrections(new Corrections)
{
}

AnalysisProcessor::AnalysisProcessor(QVector<EventDestination*> destinations, Track::Type track, Corrections::Flags flags)
  : EventProcessor(destinations)
  , m_track(0)
  , m_trackFinding(new TrackFinding)
  , m_corrections(new Corrections(flags))
{
  setTrackType(track);
  setCorrectionFlags(flags);
}

AnalysisProcessor::~AnalysisProcessor()
{
  if (m_track)
    delete m_track;
  delete m_trackFinding;
  delete m_corrections;
}

void AnalysisProcessor::setTrackType(Track::Type track)
{
  if (m_track)
    delete m_track;

  if (track == Track::None)
    m_track = 0;
  if (track == Track::CenteredBrokenLine)
    m_track = new CenteredBrokenLine;
  else if (track == Track::CenteredBrokenLine2D)
    m_track = new CenteredBrokenLine2D;
  else if (track == Track::BrokenLine)
    m_track = new BrokenLine;
  else if (track == Track::StraightLine)
    m_track = new StraightLine;

  m_particle->setTrack(m_track);
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
  m_particle->setType(Particle::Proton);
  if (m_track) {
    m_track->fit(trackClusters);
    m_corrections->postFitCorrections(m_track);
    m_track->process();
  }
  foreach (EventDestination* destination, m_destinations)
    destination->processEvent(clusters, m_particle, event);
}
