#include "Cluster.hh"

#include "Hit.hh"
#include "Cluster.hh"
#include "TOFSipmHit.hh"
#include "TOFCluster.hh"

#include <cmath>

ClassImp(Cluster);

Cluster::Cluster() :
  Hit(),
  m_hits()
{
}

Cluster::Cluster(const std::vector<Hit*>& hits) :
  Hit(),
  m_hits(hits)
{
  processHits();
}

Cluster::Cluster(const Cluster& other) :
  Hit(other)
{
  for (std::vector<Hit*>::const_iterator it = other.m_hits.begin(); it != other.m_hits.end(); it++) {
    Hit* hit = *it;
    if (strcmp(hit->ClassName(), "Hit") == 0)
      m_hits.push_back(new Hit(*hit));
    else if (strcmp(hit->ClassName(), "Cluster") == 0) {
      Cluster* cluster = static_cast<Cluster*>(hit);
      m_hits.push_back(new Cluster(*cluster));
    }
    else if (strcmp(hit->ClassName(), "TOFSipmHit") == 0) {
      TOFSipmHit* tofHit = static_cast<TOFSipmHit*>(hit);
      m_hits.push_back(new TOFSipmHit(*tofHit));
    }
    else if (strcmp(hit->ClassName(), "TOFCluster") == 0) {
      TOFCluster* cluster = static_cast<TOFCluster*>(hit);
      m_hits.push_back(new TOFCluster(*cluster));
    }
  }
}

Cluster::~Cluster()
{
  for (std::vector<Hit*>::iterator it = m_hits.begin(); it != m_hits.end(); it++) {
    delete *it;
  }
  m_hits.clear();
}

void Cluster::processHits()
{
  if (m_hits.size() == 0) {
    return;
  }
  
  double x  = 0., y  = 0., z  = 0.;
  double xc = 0., yc = 0., zc = 0.;
  unsigned short detId;
  double weightedMean;
  double weightedMeanC;
  double sumOfWeights;

  Hit* firstHit = m_hits.at(0);
  Hit::ModuleType type = firstHit->type();
  detId = firstHit->detId() - firstHit->channel();
  y  = firstHit->position().y();
  z  = firstHit->position().z();
  yc = firstHit->counterPosition().y();
  zc = firstHit->counterPosition().z();

  for (std::vector<Hit*>::iterator it = m_hits.begin(); it != m_hits.end(); it++) {
    x = (*it)->position().x();
    xc = (*it)->counterPosition().x();

    double weight = pow((*it)->signalHeight(), 2.);
    weightedMean += x*weight;
    weightedMeanC += xc*weight;
    sumOfWeights += weight;
  }
  weightedMean /= sumOfWeights;
  weightedMeanC /= sumOfWeights;

  x = weightedMean;
  xc = weightedMeanC;

  TVector3 position = TVector3(x,y,z);
  TVector3 counterPosition = TVector3(xc,yc,zc);

  int amplitude = 0.;
  for (std::vector<Hit*>::iterator it = m_hits.begin(); it != m_hits.end(); it++) {
    amplitude += (*it)->signalHeight();
  }  

  m_type = type;
  m_detId = detId;
  m_signalHeight = amplitude;
  m_position = position;
  m_counterPosition = counterPosition;
}

