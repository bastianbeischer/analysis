#ifndef EventProcessor_hh
#define EventProcessor_hh

#include <QVector>

class Particle;
class Hit;
class SimpleEvent;
class EventDestination;
class AnalyzedEvent;

class EventProcessor
{

public:
  EventProcessor();
  EventProcessor(QVector<EventDestination*>);
  virtual ~EventProcessor();

  void addDestination(EventDestination*);
  void clearDestinations();

  virtual void process(SimpleEvent*) = 0;
  bool tryProcessingDestination(EventDestination*, AnalyzedEvent*) const;

protected:
  QVector<EventDestination*> m_destinations;

};

#endif /* EventProcessor_hh */
