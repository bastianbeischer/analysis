#ifndef DetectorElement_hh
#define DetectorElement_hh

#include <QMap>
#include <QVector>

#include "Hit.hh"

class Cluster;

class DetectorElement
{
  
public:
  DetectorElement();
  DetectorElement(unsigned short id);
  virtual ~DetectorElement();
  
public:
  virtual QVector<Cluster*> findClusters() const = 0;

public:
  void              addHit(Hit* hit) {m_hits[hit->channel()] = hit;}
  void              clearHits()      {m_hits.clear();}
  unsigned short    nHits() const    {return m_hits.size();}

protected:
  unsigned short             m_id;

  QMap<unsigned short, Hit*> m_hits; // map from channel to Hit

};

#endif /* DetectorElement_hh */
