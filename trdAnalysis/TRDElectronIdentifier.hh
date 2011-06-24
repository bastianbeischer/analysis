#ifndef TRDELECTRONIDENTIFIER_HH
#define TRDELECTRONIDENTIFIER_HH

#include <QVector>

class SimpleEvent;
class Hit;
class Particle;

class TRDElectronIdentifier
{
public:
    virtual bool isElectronish(const QVector<Hit*>& hits, const Particle* particle, const SimpleEvent* event, bool &ok) = 0;
};

#endif // TRDELECTRONIDENTIFIER_HH
