#ifndef Layer_hh
#define Layer_hh

#include <QVector>
#include <QMap>

class SiPMArray;
class Cluster;
class Hit;

class Layer
{
  
public:
  Layer();
  Layer(double z);
  ~Layer();
  
public:
  QVector<Cluster*> clusters();
  Cluster*              bestCluster();

public:
  void addHitToArray(Hit* hit);
  void clearArrays();

public:
  double         z()       const {return m_z;}
  unsigned short nArrays() const {return m_arrays.size();}

  const char* printInfo();

private:
  double                               m_z;

  QMap<unsigned short, SiPMArray*> m_arrays;
  
};

#endif /* Layer_hh */
