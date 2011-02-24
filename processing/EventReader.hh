#ifndef EventReader_hh
#define EventReader_hh

#include "Track.hh"
#include "Corrections.hh"

#include <QThread>
#include <QVector>
#include <QMutex>

class DataChain;
class EventDestination;
class Corrections;

class EventReader : public QThread {
Q_OBJECT
public:
  EventReader(QObject* parent = 0);
  ~EventReader();
  void addDestination(EventDestination*);
  void clearDestinations();
  void start(Track::Type, Corrections::Flags, int nThreads);
  void start(Track::Type, Corrections::Flags, int nThreads, unsigned int first, unsigned int last);
  void stop();

  int progress() const;
  int buffer() const;

  void setFileList(const QString& fileName);
  void addFileList(const QString& fileName);
  void addRootFile(const QString& file);
signals:
  void numberOfEventsChanged(int);
  void eventLoopStarted();
  void eventLoopStopped();
protected:
  void run();
private:
  QMutex m_mutex;
  bool m_abort;
  Track::Type m_trackType;
  Corrections::Flags m_correctionFlags;
  int m_nThreads;
  unsigned int m_firstEvent;
  unsigned int m_lastEvent;
  int m_progress;
  int m_buffer;
  DataChain* m_chain;
  QVector<EventDestination*> m_destinations;
};

#endif
