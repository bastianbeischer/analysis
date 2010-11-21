#ifndef EventQueue_hh
#define EventQueue_hh

#include <QQueue>
#include <QMutex>

class SimpleEvent;

class EventQueue {
  public:
    EventQueue();
    ~EventQueue();
    int freeSpace();
    int numberOfEvents();
    void enqueue(SimpleEvent*);
    SimpleEvent* dequeue();
    
    static const int s_bufferSize = 1000;
  private:
    QQueue<SimpleEvent*> m_queue;
    QMutex m_mutex;
};

#endif
