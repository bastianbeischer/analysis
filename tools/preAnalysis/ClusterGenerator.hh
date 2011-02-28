#ifndef ClusterGenerator_hh
#define ClusterGenerator_hh

#include "EventProcessor.hh"

#include <QMap>
#include <QVector>

class DetectorElement;
class Hit;

typedef QMap< DetectorElement*, QMap<unsigned short, Hit*> > HitStorage;
typedef QMap< DetectorElement*, QMap<unsigned short, Hit*> >::iterator HitStorageIterator;

class ClusterGenerator :
  public EventProcessor
{
  
public:
  ClusterGenerator();
  ~ClusterGenerator();
  
  void process(SimpleEvent*);

private:
  HitStorage m_hitStorage;

};

#endif /* ClusterGenerator_hh */
