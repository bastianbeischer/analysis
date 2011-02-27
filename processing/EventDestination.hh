#ifndef EventDestination_hh
#define EventDestination_hh

#include <QVector>

class Hit;
class Track;
class SimpleEvent;

class EventDestination {
public:
  EventDestination();
  virtual ~EventDestination();
  virtual void processEvent(const QVector<Hit*>&, Track* = 0, SimpleEvent* = 0) = 0;
};

#endif
