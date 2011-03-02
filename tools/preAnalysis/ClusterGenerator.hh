#ifndef ClusterGenerator_hh
#define ClusterGenerator_hh

#include <QMap>
#include <QVector>

class DetectorElement;
class Hit;

typedef QMap< DetectorElement*,QVector<Hit*> > HitStorage;
typedef QMap< DetectorElement*,QVector<Hit*> >::const_iterator HitStorageIterator;


class ClusterGenerator
{
  
public:
  ClusterGenerator();
  ~ClusterGenerator();
  
  QVector<Hit*> findClusters(const QVector<Hit*>&);

private:
  HitStorage m_hitStorage;

};

#endif /* ClusterGenerator_hh */
