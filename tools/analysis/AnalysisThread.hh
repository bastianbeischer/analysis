#ifndef AnalysisThread_hh
#define AnalysisThread_hh

#include "Track.hh"
#include "Corrections.hh"

#include <QThread>
#include <QMutex>
#include <QVector>

class EventQueue;
class AnalysisPlot;
class Track;
class TrackFinding;

class AnalysisThread : public QThread {
  Q_OBJECT
public:
  AnalysisThread(EventQueue*, Track::Type, Corrections::Flags, const QVector<AnalysisPlot*>&, QObject* parent = 0);
  ~AnalysisThread();
  void start();
  void stop();
protected:
  void run();
private:
  EventQueue* m_queue;
  Track* m_track;
  TrackFinding* m_trackFinding;
  QVector<AnalysisPlot*> m_plots;
  Corrections::Flags m_correctionFlags;
  bool m_abort;
  QMutex m_mutex;
};

#endif
