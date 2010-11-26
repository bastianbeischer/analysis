#ifndef DetectorElement_hh
#define DetectorElement_hh

#include <QMap>
#include <QVector>

#include <TVector3.h>

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
  virtual QVector<Cluster*> findClusters() = 0;

public:
  void              addHit(Hit* hit)                {m_hits[hit->channel()] = hit;}
  void              clearHits()                     {m_hits.clear();}
  void              sortHits();
  void              debug(const QVector<Cluster*>&);

  void              setAlignmentShift(double shift) {m_alignmentShift = shift;}

public:
  unsigned short    type()           const {return m_type;}
  unsigned short    id()             const {return m_id;}
  unsigned short    nChannels()      const {return m_nChannels;}
  unsigned short    nHits()          const {return m_hits.size();}
  double            alignmentShift() const {return m_alignmentShift;}
  TVector3          position()       const {return m_position;}

  TVector3          positionForHit(const Hit* hit) const;

protected:
  Type                       m_type;
  unsigned short             m_id;
  unsigned short             m_nChannels;

  TVector3                   m_position;
  double                     m_alignmentShift;

  QMap<unsigned short, Hit*> m_hits;

};

#endif /* DetectorElement_hh */
