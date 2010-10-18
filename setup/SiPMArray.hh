#ifndef SiPMArray_hh
#define SiPMArray_hh

#include <map>

#include "Hit.hh"

class Cluster;

class SiPMArray
{
  
public:
  SiPMArray();
  SiPMArray(unsigned short detId);
  ~SiPMArray();
  
public:
  void addHit(Hit* hit) {m_hits[hit->channel()] = hit;}
  void clearHits() {m_hits.clear();}

  std::vector<Cluster*> findClusters();
  unsigned short nHits() const {return m_hits.size();}

private:
  unsigned short                 m_detId;

  std::map<unsigned short, Hit*> m_hits;
  
};

#endif /* SiPMArray_hh */
