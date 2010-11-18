#include "TrackSelection.hh"

#include "Track.hh"
#include "Hit.hh"

#include <QVector>

TrackSelection::TrackSelection()
{
}

TrackSelection::~TrackSelection()
{
}

bool TrackSelection::passes(Track* track, Flags flags)
{
  bool result = true;
  if (flags & AllTrackerLayers) {
    result = result && checkAllTrackerLayers(track);
  }
  if (flags & InsideMagnet) {
    result = result && checkInsideMagnet(track);
  }
  if (flags & HighPt) {
    result = result && checkHighPt(track);
  }
  return result;
}

bool TrackSelection::checkAllTrackerLayers(Track* track)
{
  int nTrackerHits = 0;
  foreach(Hit* hit, track->hits())
    if (hit->type() == Hit::tracker)
      ++nTrackerHits;
  if (nTrackerHits != 8)
    return false;
  return true;
}

bool TrackSelection::checkInsideMagnet(Track* track)
{
  return true;
}

bool TrackSelection::checkHighPt(Track* track)
{
  return true;
}
