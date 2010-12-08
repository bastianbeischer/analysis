#include "TrackSelection.hh"

#include "Track.hh"
#include "Hit.hh"

#include <QMap>
#include <QVector>
#include <cmath>
#include <cfloat>

TrackSelection::TrackSelection() :
  m_flags(None)
{
}

TrackSelection::~TrackSelection()
{
}

void TrackSelection::processTrack(Track* track)
{
  m_flags = None;

  if (!track)
    return;

  checkAllTrackerLayers(track);
  checkInsideMagnet(track);
  checkOutsideMagnet(track);
  checkHighPt(track);
  checkMagnetCollision(track);
}

void TrackSelection::checkAllTrackerLayers(Track* track)
{
  // count hits in each tracker layer
  QMap<double, int> counts;
  foreach(Hit* hit, track->hits()) {
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

void TrackSelection::checkInsideMagnet(Track* track)
{
  if (!track->fitGood())
    return;
  double r1 = sqrt(pow(track->x(40.), 2.) + pow(track->y(40.), 2.));
  double r2 = sqrt(pow(track->x(-40.), 2.) + pow(track->y(-40.), 2.));
  if (r1 < 75 && r2 < 75)
    m_flags |= InsideMagnet;
}

void TrackSelection::checkOutsideMagnet(Track* track)
{
  if (!track->fitGood())
    return;
  double r1 = sqrt(pow(track->x(40.), 2.) + pow(track->y(40.), 2.));
  double r2 = sqrt(pow(track->x(-40.), 2.) + pow(track->y(-40.), 2.));
  if (r1 > 120 && r2 > 120)
    m_flags |= OutsideMagnet;
}

void TrackSelection::checkHighPt(Track* track)
{
  if (!track->fitGood())
    return;
  double pt = track->pt();
  // 2GeV currently...
  if (pt > 2 || pt == DBL_MAX)
    m_flags |= HighPt;
}

void TrackSelection::checkMagnetCollision(Track* track)
{
  if (!track->fitGood())
    return;
  double x0 = track->x(0.);
  double y0 = track->y(0.);
  double r = sqrt(x0*x0 + y0*y0);
  if (r > 65 && r < 110)
    m_flags |= MagnetCollision;
}
