#ifndef PROTONREJECTOR_H
#define PROTONREJECTOR_H

#include <QVector>

class SimpleEvent;
class Hit;
class Track;

class ProtonRejector
{
public:
    ProtonRejector();

    bool isProton(const QVector<Hit*>& hits, const Track* track, const SimpleEvent* /*event*/, bool &ok) ;


};

#endif // PROTONREJECTOR_H
