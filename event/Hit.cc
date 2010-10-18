#include "Hit.hh"

#include <cmath>

ClassImp(Hit);

Hit::Hit() :
  TObject(),
  m_type(none),
  m_detId(0x0),
  m_signalHeight(0),
  m_position(TVector3()),
  m_counterPosition(TVector3())
{
}

Hit::Hit(ModuleType typ, unsigned short detId, int signalHeight, TVector3 position, TVector3 counterPosition) :
  TObject(),
  m_type(typ),
  m_detId(detId),
  m_signalHeight(signalHeight),
  m_position(position),
  m_counterPosition(counterPosition)
{
}

Hit::~Hit()
{
}

double Hit::angle() const
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

  return angle;
}

double Hit::resolutionEstimate() const
{
  double resolution = 0.;
  if (m_type == tracker) resolution = 1.;
  if (m_type == trd)     resolution = 6./sqrt(12);
  if (m_type == tof)     resolution = 50./sqrt(12);
  return resolution;
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
  // trd: bits 10-11 = 4 chambers
  else if (m_type == trd) {
    return ((m_detId >> 4)  & 0x03);
  }

  // tof and default
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
  // tof: last 6 bit = 64 channels
  else if (m_type == tof) {
    return ((m_detId)       & 0x40);
  }

  //default (shouldn't happen)
  return 0;
}
