#ifndef ProcessingThread_hh
#define ProcessingThread_hh

#include <QThread>
#include <QMutex>

class LimitedEventQueue;
class EventDestination;
class EventProcessor;

class ProcessingThread :
  public QThread
{

public:
  ProcessingThread(EventProcessor*, QObject* = 0);
  ~ProcessingThread();

  LimitedEventQueue* queue();
  EventProcessor* processor();

  void start();
  void stop();

protected:
  void run();

protected:
  LimitedEventQueue* m_queue;
  EventProcessor* m_processor;
  bool m_abort;
  QMutex m_mutex;

};

#endif /* ProcessingThread_hh */
