#ifndef DetectorElement_hh
#define DetectorElement_hh

#include <QMap>
#include <QVector>
#include <QVector3D>

#include "Hit.hh"

class Cluster;

class DetectorElement
{

public:
  enum Type {none=-1, tracker, trd, tof};

public:
  DetectorElement();
  DetectorElement(unsigned short id);
  virtual ~DetectorElement();
  
public:
  virtual const QVector<Cluster*>& findClusters() = 0;

public:
  void              addHit(Hit* hit)                {m_hits[hit->channel()] = hit;}
  void              clearHits()                     {m_hits.clear();}
  void              clearClusters()                 {m_clusters.clear();}
  void              deleteClusters();
  void              sortHits();
  void              debug();

public:
  unsigned short    type()           const {return m_type;}
  unsigned short    id()             const {return m_id;}
  unsigned short    nHits()          const {return m_hits.size();}
  double            alignmentShift() const {return m_alignmentShift;}
  QVector3D         position()       const {return m_position;}

protected:
  Type                       m_type;
  unsigned short             m_id;

  QVector3D                  m_position;
  double                     m_alignmentShift;

  QMap<unsigned short, Hit*> m_hits;
  QVector<Cluster*>          m_clusters;

};

#endif /* DetectorElement_hh */
