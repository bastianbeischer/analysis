#include "TrackInformation.hh"

#include "Track.hh"
#include "Hit.hh"

#include <QMap>
#include <QVector>
#include <cmath>
#include <cfloat>

TrackInformation::TrackInformation(const Track* track) :
  m_track(track),
  m_flags(None)
{
}

TrackInformation::~TrackInformation()
{
}

void TrackInformation::process()
{
  m_flags = None;

  if (!m_track)
    return;

  checkAllTrackerLayers();
  checkInsideMagnet();
  checkOutsideMagnet();
  checkHighPt();
  checkMagnetCollision();
  checkAlbedo();
}

void TrackInformation::checkAllTrackerLayers()
{
  // count hits in each tracker layer
  QMap<double, int> counts;
  foreach(Hit* hit, m_track->hits()) {
    if (hit->type() == Hit::tracker) {
      double z = round(hit->position().z()*100.)/100.;
      counts[z]++;
    }
  }
  // exactly 8 layers
  if (counts.size() != 8)
    return;
  // exactly 1 count in each layer
  foreach(int count, counts)
    if (count != 1)
      return;
  m_flags |= AllTrackerLayers;
}

void TrackInformation::checkInsideMagnet()
{
  if (!m_track->fitGood())
    return;
  double r1 = sqrt(pow(m_track->x(40.), 2.) + pow(m_track->y(40.), 2.));
  double r2 = sqrt(pow(m_track->x(-40.), 2.) + pow(m_track->y(-40.), 2.));
  if (r1 < 75 && r2 < 75)
    m_flags |= InsideMagnet;
}

void TrackInformation::checkOutsideMagnet()
{
  if (!m_track->fitGood())
    return;
  double r1 = sqrt(pow(m_track->x(40.), 2.) + pow(m_track->y(40.), 2.));
  double r2 = sqrt(pow(m_track->x(-40.), 2.) + pow(m_track->y(-40.), 2.));
  if (r1 > 120 && r2 > 120)
    m_flags |= OutsideMagnet;
}

void TrackInformation::checkHighPt()
{
  if (!m_track->fitGood())
    return;
  double pt = m_track->pt();
  // 2GeV currently...
  if (pt > 2 || pt == DBL_MAX)
    m_flags |= HighPt;
}

void TrackInformation::checkMagnetCollision()
{
  if (!m_track->fitGood())
    return;
  double x0 = m_track->x(0.);
  double y0 = m_track->y(0.);
  double r = sqrt(x0*x0 + y0*y0);
  if (r > 65 && r < 110)
    m_flags |= MagnetCollision;
}

void TrackInformation::checkAlbedo()
{
  if (!m_track->fitGood())
    return;
  if (m_track->beta() < 0)
    m_flags |= Albedo;
}
