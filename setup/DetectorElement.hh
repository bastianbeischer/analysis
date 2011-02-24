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
  virtual QVector<Hit*> findClusters(const QVector<Hit*>&) = 0;

public:
  void setAlignmentShift(double shift) {m_alignmentShift = shift;}

public:
  unsigned short type() const {return m_type;}
  unsigned short id() const {return m_id;}
  unsigned short nChannels() const {return m_nChannels;}
  double alignmentShift() const {return m_alignmentShift;}
  TVector3 position() const {return m_position;}

  void initializeChannelMap();
  TVector3 positionForHit(const Hit* hit) const;
  unsigned short sortedChannel(const unsigned short channel) const;

protected:
  Type m_type;
  unsigned short m_id;
  unsigned short m_nChannels;

  TVector3 m_position;
  double m_alignmentShift;

  QVector<unsigned short> m_channelMap;

};

#endif /* DetectorElement_hh */
