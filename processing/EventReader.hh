#ifndef EventReader_hh
#define EventReader_hh

#include <QThread>
#include <QVector>
#include <QMutex>

class DataChain;
class EventDestination;
class EventProcessor;
class ProcessingThread;
class Corrections;

class EventReader : public QThread {
Q_OBJECT
public:
  EventReader(QObject* parent = 0);
  ~EventReader();
  void start(QVector<EventProcessor*>&);
  void start(QVector<EventProcessor*>&, unsigned int first, unsigned int last);
  void stop();

  int queuedEvents() const;
  double progress() const;
  double buffer() const;

  int bufferSize() const;
  void setBufferSize(int bufferSize);

  void setFileList(const QString& fileName);
  void addFileList(const QString& fileName);
  void addRootFile(const QString& file);
signals:
  void numberOfEventsChanged(int);
protected:
  void run();
private:
  QMutex m_mutex;
  bool m_abort;
  unsigned int m_firstEvent;
  unsigned int m_lastEvent;
  unsigned int m_nEvents;
  unsigned int m_readEvents;
  DataChain* m_chain;
  int m_nThreads;
  int m_bufferSize;
  QVector<ProcessingThread*> m_threads;
};

#endif
