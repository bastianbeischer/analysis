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
  Cluster(const std::vector<Hit*>& hits);
  Cluster(const Cluster& other);
  virtual ~Cluster();

public:
  virtual void processHits();

public:
  double resolutionEstimate() const;

public:
  void addHit(Hit* hit) {m_hits.push_back(hit);}
  const std::vector<Hit*>& hits() const {return m_hits;}

protected:
  std::vector<Hit*> m_hits;

  ClassDef(Cluster, 1);

};

#endif /* Cluster_hh */
