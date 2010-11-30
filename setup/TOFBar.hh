#ifndef TOFBar_hh
#define TOFBar_hh

#include <QVector>

#include "DetectorElement.hh"

class Cluster;

class TOFBar :
  public DetectorElement
{
  
public:
  TOFBar();
  TOFBar(unsigned short detId);
  ~TOFBar();
  
public:
  QVector<Hit*> findClusters();
  
};

#endif /* TOFBar_hh */
