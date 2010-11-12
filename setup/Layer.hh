#ifndef Layer_hh
#define Layer_hh

#include <QVector>
#include <QMap>

#include "DetectorElement.hh"

class Cluster;
class Hit;

class Layer
{
  
public:
  Layer();
  Layer(double z);
  ~Layer();
  
public:
  QVector<Cluster*>     clusters()    const;
  Cluster*              bestCluster() const;

public:
  void                  addElement(DetectorElement* element) {m_elements[element->id()] = element;}
  void                  addHitToDetector(Hit* hit);
  void                  clearHitsInDetectors();
  void                  sortHits();

public:
  double                z()         const {return m_z;}
  unsigned short        nElements() const {return m_elements.size();}


  QList<unsigned short> detIds();
  bool                  contains(unsigned short detId) const;
  const char*           printInfo();

private:
  double                                 m_z;

  QMap<unsigned short, DetectorElement*> m_elements;
  
};

#endif /* Layer_hh */
