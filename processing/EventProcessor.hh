#ifndef EventProcessor_hh
#define EventProcessor_hh

#include "Track.hh"
#include "Corrections.hh"

#include <QThread>
#include <QMutex>
#include <QVector>

class EventQueue;
class TrackFinding;
class EventDestination;

class EventProcessor : public QThread {
Q_OBJECT
public:
  EventProcessor(Track::Type track, Corrections::Flags flags, QVector<EventDestination*>, QObject* parent = 0);
  ~EventProcessor();
  EventQueue* queue();
  void start();
  void stop();
protected:
  void run();
private:
  EventQueue* m_queue;
  Track* m_track;
  TrackFinding* m_trackFinding;
  Corrections* m_corrections;
  QVector<EventDestination*> m_destinations;
  bool m_abort;
  QMutex m_mutex;
};

#endif
