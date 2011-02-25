#ifndef ProcessingThread_hh
#define ProcessingThread_hh

#include <QThread>
#include <QMutex>

class EventQueue;
class EventDestination;
class EventProcessor;

class ProcessingThread : 
  public QThread
{
  
public:
  ProcessingThread(const EventProcessor*, QObject* = 0);
  ~ProcessingThread();
  
  EventQueue* queue();
  EventProcessor* processor();

  void start();
  void stop();

protected:
  void run();

protected:
  EventQueue* m_queue;
  const EventProcessor* m_processor;
  bool m_abort;
  QMutex m_mutex;
  
};

#endif /* ProcessingThread_hh */
