#include "AnalysisThread.hh"
#include "AnalysisPlot.hh"
#include "Track.hh"
#include "CenteredBrokenLine.hh"
#include "BrokenLine.hh"
#include "StraightLine.hh"
#include "TrackSelection.hh"
#include "TrackFinding.hh"
#include "Cluster.hh"
#include "Hit.hh"
#include "SimpleEvent.hh"
#include "Setup.hh"
#include "EventQueue.hh"

#include <iostream>

AnalysisThread::AnalysisThread(EventQueue* queue, Track::Type track, const QVector<AnalysisPlot*>& plots, QObject* parent)
  : QThread(parent)
  , m_queue(queue)
  , m_track(0)
  , m_trackSelection(new TrackSelection)
  , m_trackFinding(new TrackFinding)
  , m_plots(plots)
  , m_abort(true)
  , m_mutex()
{
  if (track == Track::CenteredBrokenLine)
    m_track = new CenteredBrokenLine;
  else if (track == Track::BrokenLine)
    m_track = new BrokenLine;
  else if (track == Track::StraightLine)
    m_track = new StraightLine;
}

AnalysisThread::~AnalysisThread()
{
  if (m_track)
    delete m_track;
  delete m_trackSelection;
  delete m_trackFinding;
}

void AnalysisThread::start()
{
  m_abort = false;
  QThread::start();
}

void AnalysisThread::stop()
{
  m_mutex.lock();
  m_abort = true;
  m_mutex.unlock();
  wait();
}

void AnalysisThread::run()
{
  forever {
    SimpleEvent* event = m_queue->dequeue();
    if (event) {
      QVector<Hit*> clusters;
      QVector<Hit*> hits = QVector<Hit*>::fromStdVector(event->hits());
      if (event->contentType() == SimpleEvent::Clusters)
        clusters = hits;
      else
        clusters = Setup::instance()->generateClusters(hits);

      QVector<Hit*> trackClusters = m_trackFinding->findTrack(clusters);
      if (m_track) {
        m_track->fit(trackClusters);
        m_trackSelection->processTrack(m_track);
      }
      foreach (AnalysisPlot* plot, m_plots)
        plot->processEvent(trackClusters, m_track, m_trackSelection, event);
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
