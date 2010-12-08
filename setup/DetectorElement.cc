#include "DetectorElement.hh"

#include <QtAlgorithms>

#include "Cluster.hh"

#include <cmath>
#include <cassert>
#include <iostream>

bool comparePositions(const Hit* hit1, const Hit* hit2)
{
  return hit1->position().x() < hit2->position().x();
}

DetectorElement::DetectorElement() :
  m_type(none),
  m_id(0),
  m_nChannels(0),
  m_position(TVector3(0.,0.,0.)),
  m_alignmentShift(0.)
{
}

DetectorElement::DetectorElement(unsigned short id) :
  m_type(none),
  m_id(id),
  m_nChannels(0),
  m_position(TVector3(0.,0.,0.)),
  m_alignmentShift(0.)
{
}

DetectorElement::~DetectorElement()
{
}

void DetectorElement::sortHits()
{
  // check if all channels are present --> otherwise reinserting into map will be wrong if a channel was skipped
  if (m_type == tracker)
    assert(m_hits.size() == 32);
  else if (m_type == trd)
    assert(m_hits.size() == 16);
  // else if (m_type == tof)
  //   assert(hits.size() == 4);

  QList<Hit*> hits = m_hits.values();
  qSort(hits.begin(), hits.end(), comparePositions);
  m_hits.clear();
  for (int i = 0; i < hits.size(); ++i) {
    m_hits[i] = hits.at(i);
  }
}

void DetectorElement::debug(const QVector<Hit*>& clusters)
{
  std::cout << "------------------------" << std::endl;
  int i = 0;
  std::cout << "Hits:" << std::endl;
  foreach(Hit* hit, m_hits) {
    if (hit) {
      std::cout << i << " " << hit->position().x() << "     "  << hit->signalHeight() << std::endl;
    }
    ++i;
  }
  std::cout << "Clusters:" << std::endl;
  int j = 0;
  foreach(Hit* hit, clusters) {
    i = 0;
    std::cout << "No. " << j << ":" << std::endl;
    Cluster* cluster = static_cast<Cluster*>(hit);
    foreach(Hit* hit, cluster->hits()) {
      std::cout << hit->channel() << " " << hit->signalHeight() << std::endl;
      ++i;
    }
    ++j;
  }
  // std::vector<int> channels;
  // foreach(Cluster* cluster, m_clusters) {
  //   foreach(Hit* hit, cluster->hits()) {
  //     int channel = hit->channel();
  //     if (std::find(channels.begin(), channels.end(), channel) != channels.end()) {
  //       std::cout << "BAD" << std::endl;
  //       exit(-1);
  //     }
  //     else {
  //       // std::cout << channel << std::endl;
  //       channels.push_back(hit->channel());
  //     }
  //   }
  // }
}

TVector3 DetectorElement::positionForHit(const Hit* hit) const
{
  double posX = hit->position().x();
  double posY = hit->position().y();
  double posZ = hit->position().z();
  double counterPosX = hit->counterPosition().x();
  double counterPosY = hit->counterPosition().y();

  double x = -alignmentShift();
  double y = 0.;
  double angle = hit->angle();
  double c = cos(angle);
  double s = sin(angle);
  double u = c*x - s*y;
  double v = s*x + c*y;

  return TVector3(0.5*(posX+counterPosX) + u, 0.5*(posY+counterPosY) + v, posZ);
}
