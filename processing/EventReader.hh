#ifndef EventReader_hh
#define EventReader_hh

#include <QThread>
#include <QVector>
#include <QMutex>

class DataChain;
class EventProcessor;
class Corrections;

class EventReader : public QThread {
Q_OBJECT
public:
  EventReader(DataChain*, QObject* parent = 0);
  ~EventReader();
  void addEventProcessor(EventProcessor*);
  void start(unsigned int first, unsigned int last);
  void stop();
signals:
  void progressChanged(int);
  void bufferChanged(int);
  void eventLoopStarted();
  void eventLoopStopped();
protected:
  void run();
private:
  QMutex m_mutex;
  bool m_abort;
  unsigned int m_firstEvent;
  unsigned int m_lastEvent;
  int m_progress;
  int m_buffer;
  DataChain* m_chain;
  QVector<EventProcessor*> m_processors;
};

#endif
