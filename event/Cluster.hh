#ifndef Cluster_hh
#define Cluster_hh

#include <vector>

#include "Hit.hh"
#include <TVector3.h>

class Cluster :
  public Hit
{
  
public:
  Cluster();
  Cluster(std::vector<Hit*> hits);
  ~Cluster();
  
public:
  void               processHits();

public:
  void               addHit(Hit* hit) {m_hits.push_back(hit);}
  std::vector<Hit*>& hits() {return m_hits;}

private:
  std::vector<Hit*> m_hits;
  
  ClassDef(Cluster, 1);

};

#endif /* Cluster_hh */
