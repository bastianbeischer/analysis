#include "EventProcessor.hh"

#include "Track.hh"
#include "TrackFinding.hh"
#include "Corrections.hh"
#include "CenteredBrokenLine.hh"
#include "CenteredBrokenLine2D.hh"
#include "BrokenLine.hh"
#include "StraightLine.hh"
#include "TrackFinding.hh"
#include "Cluster.hh"
#include "Hit.hh"
#include "SimpleEvent.hh"
#include "Setup.hh"
#include "EventQueue.hh"
#include "EventDestination.hh"

EventProcessor::EventProcessor(Track::Type track, Corrections::Flags flags, QVector<EventDestination*> destinations, QObject* parent)
  : QThread(parent)
  , m_queue(new EventQueue)
  , m_track(0)
  , m_trackFinding(new TrackFinding)
  , m_corrections(new Corrections(flags))
  , m_destinations(destinations)
  , m_abort(true)
  , m_mutex()
{
  if (track == Track::CenteredBrokenLine)
    m_track = new CenteredBrokenLine;
  else if (track == Track::CenteredBrokenLine2D)
    m_track = new CenteredBrokenLine2D;
  else if (track == Track::BrokenLine)
    m_track = new BrokenLine;
  else if (track == Track::StraightLine)
    m_track = new StraightLine;
}

EventProcessor::~EventProcessor()
{
  delete m_queue;
  if (m_track)
    delete m_track;
  delete m_trackFinding;
  delete m_corrections;
}

EventQueue* EventProcessor::queue()
{
  QMutexLocker locker(&m_mutex);
  return m_queue;
}

void EventProcessor::start()
{
  m_abort = false;
  QThread::start();
}

void EventProcessor::stop()
{
  m_mutex.lock();
  m_abort = true;
  m_mutex.unlock();
  wait();
}

void EventProcessor::run()
{
  forever {
    SimpleEvent* event = m_queue->dequeue();
    if (event) {
      m_corrections->preFitCorrections(event);

      QVector<Hit*> clusters;
      QVector<Hit*> hits = QVector<Hit*>::fromStdVector(event->hits());
      if (event->contentType() == SimpleEvent::Clusters)
        clusters = hits;
      else
        clusters = Setup::instance()->generateClusters(hits);

      QVector<Hit*> trackClusters = m_trackFinding->findTrack(clusters);
      if (m_track) {
        m_track->fit(trackClusters);
        m_corrections->postFitCorrections(m_track);
        m_track->process();
      }
      foreach (EventDestination* destination, m_destinations)
        destination->processEvent(clusters, m_track, event);
      if (event->contentType() == SimpleEvent::RawData)
        qDeleteAll(clusters);
      delete event;
    } else {
      usleep(1000);
    }
    m_mutex.lock();
    if (m_abort) {
      m_mutex.unlock();
      return;
    }
    m_mutex.unlock();
  }
}
