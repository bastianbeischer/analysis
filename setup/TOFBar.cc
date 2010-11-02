#include "TOFBar.hh"

TOFBar::TOFBar() :
  DetectorElement()
{
  m_type = tof;
}

TOFBar::~TOFBar()
{
}

QVector<Cluster*> TOFBar::findClusters() const
{
  QVector<Cluster*> clusters;
  return clusters;
}
