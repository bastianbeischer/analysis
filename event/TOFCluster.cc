#include "TOFCluster.hh"

ClassImp(TOFCluster);

TOFCluster::TOFCluster()
	: Cluster()
{}

TOFCluster::TOFCluster(std::vector<Hit*> hits)
	: Cluster(hits)
{
	processHits();
}

TOFCluster::~TOFCluster()
{}

void TOFCluster::processHits()
{
	m_hits.size();
	//startTime
  /*
	double x  = 0., y  = 0., z  = 0.;
  double xc = 0., yc = 0., zc = 0.;
  unsigned short detId;
  double weightedMean;
  double weightedMeanC;
  double sumOfWeights;

  if (m_hits.size() == 0) {
    return;
  }
  
  Hit* firstHit = m_hits.at(0);
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

  m_type = Hit::tracker;
  m_detId = detId;
  m_signalHeight = amplitude;
  m_position = position;
  m_counterPosition = counterPosition;

  // m_outputHit = new Hit(Hit::tracker, detId, amplitude, position, counterPosition);
	*/
}

