#include "DetectorElement.hh"

#include <QtAlgorithms>
#include <QString>

#include "Setup.hh"
#include "Cluster.hh"

#include <cmath>
#include <iostream>

DetectorElement::DetectorElement(Type type, unsigned short id, unsigned short nChannels, const Setup* setup) :
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

  m_position = setup->configFilePosition(configGroup, m_id | (m_nChannels/2));
  m_alignmentShift = setup->configFileAlignmentShift(configGroup, m_id);

  if (m_type != tof) {
    QMap<double, unsigned short> temporaryMap;
    for (int channel = 0; channel < m_nChannels; channel++)
      temporaryMap[setup->configFilePosition(configGroup, m_id | channel).x()] = channel;
    m_channelMap = temporaryMap.values().toVector();
  }
  else {
    for (int i = 0; i < m_nChannels; i++) {
      m_channelMap.push_back(i);
    }
  } 
}

DetectorElement::~DetectorElement()
{
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
