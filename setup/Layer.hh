#ifndef Layer_hh
#define Layer_hh

#include <QVector>
#include <QMap>

class DetectorElement;
class Cluster;
class Hit;

class Layer
{
  
public:
  Layer();
  Layer(double z);
  ~Layer();
  
public:
  QVector<Cluster*> clusters()    const;
  Cluster*          bestCluster() const;

public:
  void              addHitToDetector(Hit* hit);
  void              clearHitsInDetectors();

public:
  double            z()         const {return m_z;}
  unsigned short    nElements() const {return m_elements.size();}

  const char*       printInfo();

private:
  double                    m_z;

  QVector<DetectorElement*> m_elements;
  
};

#endif /* Layer_hh */
