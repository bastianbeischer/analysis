#ifndef TRDELECTRONIDENTIFIER_HH
#define TRDELECTRONIDENTIFIER_HH

#include <QVector>

class SimpleEvent;
class Hit;
class Particle;
class AnalyzedEvent;

class TRDElectronIdentifier
{
public:
  virtual bool isElectronish(const AnalyzedEvent*, bool &ok) = 0;
};

#endif // TRDELECTRONIDENTIFIER_HH
