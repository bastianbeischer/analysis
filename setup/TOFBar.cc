#include "TOFBar.hh"

#include "TOFCluster.hh"
#include "TOFSipmHit.hh"
#include "Setup.hh"

#include <QString>

TOFBar::TOFBar(unsigned short detId)
  : DetectorElement(tof, detId, 4)
{
  m_timeShifts = QVector<double>(m_nChannels, 0);
  for(unsigned short channel = 0; channel < m_nChannels; ++channel)
    m_timeShifts[channel] = Setup::instance()->configFileTimeShift(m_id | channel);
}

TOFBar::~TOFBar()
{
}

QVector<Hit*> TOFBar::findClusters()
{
  QVector<Hit*> clusters;
  if(m_hits.size() >= 3) {
    TOFCluster* tofCluster = new TOFCluster;
    foreach(Hit* hit, m_hits) {
      TOFSipmHit* tofHit = static_cast<TOFSipmHit*>(hit);
      tofCluster->addHit(new TOFSipmHit(*tofHit));
    }
    tofCluster->processHits();
    clusters.push_back(tofCluster);
  }
  return clusters;
}

double TOFBar::timeShift(unsigned short channel)
{
  Q_ASSERT(channel < m_nChannels);
  return m_timeShifts[channel];
}

void TOFBar::setTimeShifts(double ch0, double ch1, double ch2, double ch3)
{
  m_timeShifts[0] = ch0;
  m_timeShifts[1] = ch1;
  m_timeShifts[2] = ch2;
  m_timeShifts[3] = ch3;
}
