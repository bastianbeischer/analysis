#ifndef SipmArray_hh
#define SipmArray_hh

#include "DetectorElement.hh"

class SipmArray :
  public DetectorElement
{
  
public:
  explicit SipmArray(unsigned short detId, Setup* setup);
  ~SipmArray();
  
public:
  QVector<Hit*> findClusters(); // these clusters have to be deleted by hand!

};

#endif /* SipmArray_hh */
