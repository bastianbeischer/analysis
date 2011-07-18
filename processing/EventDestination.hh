#ifndef EventDestination_hh
#define EventDestination_hh

#include <QVector>
#include <QMutex>

class Hit;
class Particle;
class SimpleEvent;

class EventDestination {
public:
  EventDestination();
  virtual ~EventDestination();
  virtual void processEvent(const QVector<Hit*>&, Particle* = 0, SimpleEvent* = 0) = 0;
  bool needsLocking() const {return m_needsLocking;}
  void setNeedsLocking(bool value) {m_needsLocking = value;}
  bool tryLock();
  void lock();
  void unlock();

protected:
  bool m_needsLocking;
  QMutex m_mutex;
};

#endif
