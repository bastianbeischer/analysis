#include "TrackSelection.hh"

#include "Track.hh"
#include "Hit.hh"

#include <QVector>
#include <cmath>
#include <cfloat>

TrackSelection::TrackSelection() :
  m_flags(TrackSelection::None)
{
}

TrackSelection::~TrackSelection()
{
}

void TrackSelection::processTrack(Track* track)
{
  if (!track)
    return;

  m_flags = None;

  checkAllTrackerLayers(track);
  checkInsideMagnet(track);
  checkHighPt(track);
}

void TrackSelection::checkAllTrackerLayers(Track* track)
{
  int nTrackerHits = 0;
  foreach(Hit* hit, track->hits())
    if (hit->type() == Hit::tracker)
      ++nTrackerHits;
  if (nTrackerHits == 8)
    m_flags = static_cast<Flags>(m_flags | AllTrackerLayers);
}

void TrackSelection::checkInsideMagnet(Track* track)
{
  double r1 = sqrt(pow(track->x(40.), 2.) + pow(track->y(40.), 2.));
  double r2 = sqrt(pow(track->x(-40.), 2.) + pow(track->y(-40.), 2.));
  if (r1 < 7.5 && r2 < 7.5)
    m_flags = static_cast<Flags>(m_flags | InsideMagnet);
}

void TrackSelection::checkHighPt(Track* track)
{
  double pt = track->pt();
  // 2GeV currently...
  if (pt > 2 || pt == DBL_MAX)
    m_flags = static_cast<Flags>(m_flags | HighPt);
}

void TrackSelection::checkMagnetCollision(Track* track)
{
  // only select tracks which didn't pass through the magnet
  double x0 = track->x(0.);
  double y0 = track->y(0.);
  double r = sqrt(x0*x0 + y0*y0);
  if (r > 65 && r < 110)
    m_flags = static_cast<Flags>(m_flags | MagnetCollision);
}
