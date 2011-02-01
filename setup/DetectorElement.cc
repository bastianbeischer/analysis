#include "DetectorElement.hh"

#include <QtAlgorithms>
#include <QString>

#include "Setup.hh"
#include "Cluster.hh"

#include <cmath>
#include <cassert>
#include <iostream>

DetectorElement::DetectorElement(Type type, unsigned short id, unsigned short nChannels) :
  m_type(type),
  m_id(id),
  m_nChannels(nChannels),
  m_position(TVector3(0.,0.,0.)),
  m_alignmentShift(0.)
{
  QString configGroup;
  if (m_type == tracker) configGroup = "tracker";
  if (m_type == trd) configGroup = "trd";
  if (m_type == tof) configGroup = "tof";

  m_position = Setup::instance()->configFilePosition(configGroup, m_id | (m_nChannels/2));
  m_alignmentShift = Setup::instance()->configFileAlignmentShift(configGroup, m_id);
  QMap<double, unsigned short> temporaryMap;
  for (int channel = 0; channel < m_nChannels; channel++)
    temporaryMap[Setup::instance()->configFilePosition(configGroup, m_id | channel).x()] = channel;
  m_channelMap = temporaryMap.values().toVector();
}

DetectorElement::~DetectorElement()
{
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

unsigned short DetectorElement::sortedChannel(const unsigned short channel) const
{
  Q_ASSERT(channel < m_channelMap.size());
  return m_channelMap.at(channel);
}
