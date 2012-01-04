#ifndef LimitedEventQueue_hh
#define LimitedEventQueue_hh

#include "EventQueue.hh"

class LimitedEventQueue : public EventQueue
{

public:
  LimitedEventQueue(int = 1000);
  ~LimitedEventQueue();

  int freeSpace();
  int bufferSize() const {return m_bufferSize;}
  void setBufferSize(int size) {m_bufferSize = size;}

private:
  int m_bufferSize;

};

#endif /* LimitedEventQueue_hh */
