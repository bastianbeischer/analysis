#include "TOFCluster.hh"
#include "TOFSipmHit.hh"

#include <cmath>
#include <cassert>

#include <iostream>

ClassImp(TOFCluster);

TOFCluster::TOFCluster()
  : Cluster()
  , m_yEstimate(0)
  , m_yResolutionEstimate(395./sqrt(12.))
{}

TOFCluster::TOFCluster(std::vector<Hit*> hits)
  : Cluster(hits)
{
}

TOFCluster::~TOFCluster()
{}

void TOFCluster::processHits()
{
  int nLeft = 0;
  int nRight = 0;
  double sumLeft = .0;
  double sumRight = .0;
  for (unsigned int i = 0; i < m_hits.size(); ++i) {
    assert(m_hits[i]->type() == Hit::tof);
    if (m_hits[i]->position().y() < 0) {
      sumLeft+= static_cast<TOFSipmHit*>(m_hits[i])->startTime();
      ++nLeft;
    }
    if (m_hits[i]->position().y() > 0) {
      sumRight+= static_cast<TOFSipmHit*>(m_hits[i])->startTime();
      ++nRight;
    }
  }
  double x, y, z;
  double xc, yc, zc;
  if (!m_hits.size())
    return;
  Hit* firstHit = m_hits[0];
  unsigned short detId = firstHit->detId() - firstHit->channel();
  x = 50.*int(firstHit->position().x()/50.) + (firstHit->position().x() < 0 ? -25 : 25);
  y = firstHit->position().y();
  z = firstHit->position().z();
  xc = x;
  yc = firstHit->counterPosition().y();
  zc = firstHit->counterPosition().z();
  if (nLeft && nRight) {
    double dt = sumLeft/nLeft - sumRight/nRight; // ns
    //const double barLength = 395; // mm
    const double refractiveIndex = 1.58;
    const double speedOfLight = 299792458; // m/s
    m_yEstimate = 0.5e-6 * refractiveIndex * speedOfLight * dt;
    double sigmaDt = 0.6; //TODO replace with function of m_yEstimate.
    m_yResolutionEstimate = 0.5e-6 * refractiveIndex * speedOfLight * sigmaDt;
  }

  TVector3 position = TVector3(x,y,z);
  TVector3 counterPosition = TVector3(xc,yc,zc);

  int signalHeight = 0;
  for (std::vector<Hit*>::iterator it = m_hits.begin(); it != m_hits.end(); it++) {
    signalHeight+= (*it)->signalHeight();
  }  

  m_type = Hit::tof;
  m_detId = detId;
  m_signalHeight = signalHeight;
  m_position = position;
  m_counterPosition = counterPosition;
}

double TOFCluster::yEstimate(bool forceIntoBar)
{
  if (forceIntoBar && m_yEstimate > 395./2)
    return 395./2;
  else if (forceIntoBar && m_yEstimate < -395./2)
    return -395./2;
  return m_yEstimate;
}

void TOFCluster::dump()
{
  std::cout << m_hits.size() << " hits in cluster." << std::endl << std::flush;
  for (unsigned int i = 0; i < m_hits.size(); ++i) {
    static_cast<TOFSipmHit*>(m_hits[i])->dump();
  }
} 
