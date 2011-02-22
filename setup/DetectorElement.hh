#ifndef DetectorElement_hh
#define DetectorElement_hh

#include <QMap>
#include <QVector>

#include <TVector3.h>

#include "Hit.hh"

class Cluster;
class Setup;

class DetectorElement
{

public:
  enum Type {none=-1, tracker, trd, tof};

public:
  explicit DetectorElement(Type type, unsigned short id, unsigned short nChannels, const Setup* setup);
  virtual ~DetectorElement();
  
public:
  virtual QVector<Hit*>      findClusters() = 0;

public:
  void                       addHit(Hit* hit)                {m_hits[sortedChannel(hit->channel())] = hit;}
  void                       clearHits()                     {m_hits.clear();}
  void                       debug(const QVector<Hit*>&);

  void                       setAlignmentShift(double shift) {m_alignmentShift = shift;}

public:
  unsigned short             type()           const {return m_type;}
  unsigned short             id()             const {return m_id;}
  unsigned short             nChannels()      const {return m_nChannels;}
  unsigned short             nHits()          const {return m_hits.size();}
  double                     alignmentShift() const {return m_alignmentShift;}
  TVector3                   position()       const {return m_position;}

  void                       initializeChannelMap();
  TVector3                   positionForHit(const Hit* hit) const;
  unsigned short             sortedChannel(const unsigned short channel) const;

protected:
  Type                       m_type;
  unsigned short             m_id;
  unsigned short             m_nChannels;

  TVector3                   m_position;
  double                     m_alignmentShift;

  QMap<unsigned short, Hit*> m_hits;
  QVector<unsigned short>    m_channelMap;

};

#endif /* DetectorElement_hh */
