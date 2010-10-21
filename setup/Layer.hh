#ifndef Layer_hh
#define Layer_hh

#include <QVector>
#include <QMap>

class SipmArray;
class Cluster;
class Hit;

class Layer
{
  
public:
  Layer();
  Layer(double z);
  ~Layer();
  
public:
  QVector<Cluster*> clusters() const;
  Cluster*          bestCluster() const;

public:
  void              addHitToArray(Hit* hit);
  void              clearArrays();

public:
  double            z()       const {return m_z;}
  unsigned short    nArrays() const {return m_arrays.size();}

  const char*       printInfo();

private:
  double              m_z;

  QVector<SipmArray*> m_arrays;
  
};

#endif /* Layer_hh */
