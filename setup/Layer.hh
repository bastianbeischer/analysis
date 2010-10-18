#ifndef Layer_hh
#define Layer_hh

#include <vector>
#include <map>

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
  std::vector<Cluster*> clusters();
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

  std::map<unsigned short, SiPMArray*> m_arrays;
  
};

#endif /* Layer_hh */
