#ifndef EventDestination_hh
#define EventDestination_hh

#include "AnalyzedEvent.hh"
#include "EventFlagFilter.hh"

#include <QVector>
#include <QMutex>

class Hit;
class Particle;
class SimpleEvent;

class EventDestination {
public:
  EventDestination();
  virtual ~EventDestination();
  virtual void processEvent(const AnalyzedEvent*) = 0;
  bool needsLocking() const {return m_needsLocking;}
  void setNeedsLocking(bool value) {m_needsLocking = value;}
  bool tryLock();
  void lock();
  void unlock();
  void addRequiredEventFlags(Enums::EventFlags flags) {m_filter.addRequired(flags);}
  void addRequiredEventFlagsAbsence(Enums::EventFlags flags) {m_filter.addRequiredAbsence(flags);}
  void setEventFlagFilter(const EventFlagFilter& filter) {m_filter = filter;}
  const EventFlagFilter& eventFlagFilter() const {return m_filter;}
private:
  bool m_needsLocking;
  QMutex m_mutex;
  EventFlagFilter m_filter;
};

#endif
