#ifndef TOFCluster_hh
#define TOFCluster_hh

#include "Cluster.hh"

class TOFCluster : public Cluster {
  public:
    TOFCluster();
    TOFCluster(std::vector<Hit*> hits);
    virtual ~TOFCluster();
    virtual void processHits();
    double yEstimate() {return m_yEstimate;}
    double yResolutionEstimate() {return m_yResolutionEstimate;}
    void dump();
  private:
    double m_yEstimate;
    double m_yResolutionEstimate;
  ClassDef(TOFCluster, 1);
};

#endif
