#ifndef SiPMArray_hh
#define SiPMArray_hh

#include <QVector>
#include <QMap>

#include "DetectorElement.hh"
#include "Hit.hh"

class Cluster;

class SiPMArray :
  public DetectorElement
{
  
public:
  SiPMArray();
  SiPMArray(unsigned short detId);
  ~SiPMArray();
  
public:
  void addHit(Hit* hit) {m_hits[hit->channel()] = hit;}
  void clearHits() {m_hits.clear();}

  QVector<Cluster*> findClusters();
  unsigned short nHits() const {return m_hits.size();}

private:
  QMap<unsigned short, Hit*> m_hits;
  
};

#endif /* SiPMArray_hh */
