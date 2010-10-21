#ifndef SipmArray_hh
#define SipmArray_hh

#include <QVector>
#include <QMap>

#include "DetectorElement.hh"
#include "Hit.hh"

class Cluster;

class SipmArray :
  public DetectorElement
{
  
public:
  SipmArray();
  SipmArray(unsigned short detId);
  ~SipmArray();
  
public:
  QVector<Cluster*> findClusters() const; // these clusters have to be deleted by hand!

public:
  void              addHit(Hit* hit) {m_hits[hit->channel()] = hit;}
  void              clearHits()      {m_hits.clear();}

public:
  unsigned short    nHits() const {return m_hits.size();}

private:
  QMap<unsigned short, Hit*> m_hits; // map from channel to Hit
  
};

#endif /* SipmArray_hh */
