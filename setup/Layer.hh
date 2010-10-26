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

  bool              contains(unsigned short detId) const;
  const char*       printInfo();

private:
  double                                 m_z;

  QMap<unsigned short, DetectorElement*> m_elements;
  
};

#endif /* Layer_hh */
