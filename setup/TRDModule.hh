#ifndef TRDModule_hh
#define TRDModule_hh

#include "DetectorElement.hh"

class TRDModule :
  public DetectorElement
{
  
public:
  explicit TRDModule(unsigned short detId, Setup*);
  ~TRDModule();
  
public:
  QVector<Hit*> findClusters(); // these clusters have to be deleted by hand!

};

#endif /* TRDModule_hh */
