#ifndef TOFCluster_hh
#define TOFCluster_hh

#include "Cluster.hh"

class TOFCluster : public Cluster {
	public:
		TOFCluster();
		TOFCluster(std::vector<Hit*> hits);
		virtual ~TOFCluster();
		virtual void processHits();
	private:

	ClassDef(Cluster, 1);
};

#endif
