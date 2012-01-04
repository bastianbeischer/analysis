#include "Cluster.hh"

#include "Hit.hh"
#include "Cluster.hh"
#include "TOFSipmHit.hh"
#include "TOFCluster.hh"

#include <cmath>
#include <cassert>

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
  unsigned short detId = 0;
  double weightedMeanX = 0.;
  double weightedMeanXC = 0.;
  double weightedMeanY = 0.;
  double weightedMeanYC = 0.;
  double sumOfWeights = 0.;
  double amplitude = 0.;

  Hit* firstHit = m_hits.at(0);
  Hit::ModuleType type = firstHit->type();
  z  = firstHit->position().z();
  zc = firstHit->counterPosition().z();

  double max = 0;
  for (unsigned int i = 0 ; i < m_hits.size(); i++) {
    Hit* hit = m_hits.at(i);
    double signalHeight = hit->signalHeight();
    double weight = pow(signalHeight, 2.);
    x = hit->position().x();
    xc = hit->counterPosition().x();
    y = hit->position().y();
    yc = hit->counterPosition().y();
    weightedMeanX += x*weight;
    weightedMeanXC += xc*weight;
    weightedMeanY += y*weight;
    weightedMeanYC += yc*weight;
    sumOfWeights += weight;
    amplitude += signalHeight;
    if (signalHeight > max) {
      max = signalHeight;
      detId = hit->detId();
    }
  }
  assert(detId);

  x = weightedMeanX / sumOfWeights;
  xc = weightedMeanXC / sumOfWeights;
  y = weightedMeanY / sumOfWeights;
  yc = weightedMeanYC / sumOfWeights;

  m_type = type;
  m_detId = detId;
  m_signalHeight = amplitude;
  m_position = TVector3(x,y,z);
  m_counterPosition = TVector3(xc,yc,zc);

  calculateAngle();
}

double Cluster::resolutionEstimate() const
{
  if (m_type == tracker) {
    int length = m_hits.size();
    if (length == 1) return .250/sqrt(12);
    else if (length == 2) return .061;
    else if (length >= 3) return .050;
  }
  return Hit::resolutionEstimate();
}
