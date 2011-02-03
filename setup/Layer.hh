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
  QVector<Hit*>         clusters()    const;
  Cluster*              bestCluster() const;

public:
  void                  addElement(DetectorElement* element) {m_elements[element->id()] = element;}
  void                  addHitToDetector(Hit* hit);
  void                  clearHitsInDetectors();

public:
  double                z()         const {return m_z;}
  QList<DetectorElement*> elements() const {return m_elements.values();}
  unsigned short        nElements() const {return m_elements.size();}


  const QList<unsigned short>& detIds() const {return m_ids;};
  bool                  contains(unsigned short detId) const;
  const char*           printInfo() const;
  void                  sortIdsByPosition();

private:
  double                                 m_z;

  QMap<unsigned short, DetectorElement*> m_elements;
  QList<unsigned short>                  m_ids;

};

#endif /* Layer_hh */
