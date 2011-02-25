#ifndef EventProcessor_hh
#define EventProcessor_hh

#include <QVector>

class SimpleEvent;
class EventDestination;

class EventProcessor
{
  
public:
  EventProcessor();
  EventProcessor(QVector<EventDestination*>);
  virtual ~EventProcessor();
  
  void addDestination(EventDestination*);
  void clearDestinations();

  virtual void process(SimpleEvent*) const = 0;

protected:
  QVector<EventDestination*> m_destinations;

};

#endif /* EventProcessor_hh */
