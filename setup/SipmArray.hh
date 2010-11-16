#ifndef SipmArray_hh
#define SipmArray_hh

#include "DetectorElement.hh"

class SipmArray :
  public DetectorElement
{
  
public:
  SipmArray();
  SipmArray(unsigned short detId);
  ~SipmArray();
  
public:
  const QVector<Cluster*>& findClusters(); // these clusters have to be deleted by hand!

};

#endif /* SipmArray_hh */
