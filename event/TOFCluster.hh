#ifndef TOFCluster_hh
#define TOFCluster_hh

#include "Cluster.hh"

class TOFCluster : public Cluster {
  public:
    TOFCluster();
    TOFCluster(const std::vector<Hit*>& hits);
    TOFCluster(const TOFCluster& other);
    const TOFCluster& operator=(const TOFCluster& right);
    virtual ~TOFCluster();
    virtual void processHits();
    double yEstimate(bool forceIntoBar = true);
    double yResolutionEstimate() {return m_yResolutionEstimate;}
    void dump();
  private:
    double m_yEstimate;
    double m_yResolutionEstimate;
  ClassDef(TOFCluster, 1);
};

#endif
