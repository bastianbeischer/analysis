#include "Hit.hh"

#include <cmath>

ClassImp(Hit);

Hit::Hit() :
  TObject(),
  m_type(none),
  m_detId(0x0),
  m_signalHeight(0.),
  m_position(TVector3()),
  m_counterPosition(TVector3()),
  m_angle(0.)
{
}

Hit::Hit(ModuleType typ, unsigned short detId, double signalHeight, TVector3 position, TVector3 counterPosition) :
  TObject(),
  m_type(typ),
  m_detId(detId),
  m_signalHeight(signalHeight),
  m_position(position),
  m_counterPosition(counterPosition),
  m_angle(0.)
{
  calculateAngle();
}

Hit::Hit(const Hit& other) :
  TObject(other),
  m_type(other.m_type),
  m_detId(other.m_detId),
  m_signalHeight(other.m_signalHeight),
  m_position(other.m_position),
  m_counterPosition(other.m_counterPosition),
  m_angle(other.m_angle)
{
}

Hit::~Hit()
{
}

void Hit::calculateAngle()
{
  // y-axis
  TVector3 axis(0.,1.,0.);
  // direction of fiber
  TVector3 dir = m_position - m_counterPosition;
  // make direction point into positive y axis for angle calculation
  if (dir.y() < 0)
    dir = -dir;
  // Angle is always > 0, take sign into account
  double angle = dir.Angle(axis);
  if (dir.x() > 0)
    angle = -angle;

  m_angle = angle;
}

double Hit::resolutionEstimate() const
{
  if (m_type == tracker) return 50e-3;
  if (m_type == trd)     return 6./sqrt(12);
  if (m_type == tof)     return 50./sqrt(12);
  return 0;
}

unsigned short Hit::device() const
{
  // usbboard: first 5 bit
  return ((m_detId >> 11) & 0x1F);
}

unsigned short Hit::connection() const
{
  // connection: bits 6-8
  return ((m_detId >> 8)  & 0x07);
}

unsigned short Hit::block() const
{
  // tracker: bits 9-11 = 8 VA32
  if (m_type == tracker) {
    return ((m_detId >> 5)  & 0x07);
  }
  // trd: bits 11-12 = 4 chambers
  else if (m_type == trd) {
    return ((m_detId >> 4)  & 0x03);
  }
  // tof: bits 11-14 = 16 bars
  else if (m_type == trd) {
    return ((m_detId >> 2)  & 0x0F);
  }

  // default (shouldn't happen)
  return 0;
}

unsigned short Hit::channel() const
{
  // tracker: last 5 bit = 32 channels
  if (m_type == tracker) {
    return ((m_detId)       & 0x1F);
  }
  // trd: last 4 bit = 16 channels
  else if (m_type == trd) {
    return ((m_detId)       & 0x0F);
  }
  // tof: last 2 bit = 4 SiPMs on this bar
  else if (m_type == tof) {
    return ((m_detId)       & 0x03);
  }

  //default (shouldn't happen)
  return 0;
}
