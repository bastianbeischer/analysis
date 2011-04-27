#ifndef EventQueue_hh
#define EventQueue_hh

#include <QQueue>
#include <QMutex>

class SimpleEvent;

class EventQueue {
public:
  EventQueue();
  ~EventQueue();
  int numberOfEvents();
  void enqueue(SimpleEvent*);
  SimpleEvent* dequeue();
    
protected:
  QQueue<SimpleEvent*> m_queue;
  QMutex m_mutex;
};

#endif
