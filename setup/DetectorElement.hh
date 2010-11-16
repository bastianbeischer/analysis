#ifndef DetectorElement_hh
#define DetectorElement_hh

#include <QMap>
#include <QVector>

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
  void              setAlignmentShift(double shift) {m_alignmentShift = shift;}
  void              sortHits();
  void              debug();

public:
  unsigned short    type()           const {return m_type;}
  unsigned short    id()             const {return m_id;}
  unsigned short    nHits()          const {return m_hits.size();}
  double            alignmentShift() const {return m_alignmentShift;}

protected:
  Type                       m_type;
  unsigned short             m_id;

  double                     m_alignmentShift;

  QMap<unsigned short, Hit*> m_hits;
  QVector<Cluster*>          m_clusters;

};

#endif /* DetectorElement_hh */
