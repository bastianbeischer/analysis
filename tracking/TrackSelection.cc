#include "TrackSelection.hh"

#include "Track.hh"
#include "Hit.hh"

#include <QVector>

TrackSelection::TrackSelection() :
  m_flags(TrackSelection::None)
{
}

TrackSelection::~TrackSelection()
{
}

void TrackSelection::processTrack(Track* track)
{
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
  if (nTrackerHits != 8)
    return;

  m_flags = static_cast<Flags>(m_flags | AllTrackerLayers);
}

void TrackSelection::checkInsideMagnet(Track* /*track*/)
{
  m_flags = static_cast<Flags>(m_flags | InsideMagnet);
}

void TrackSelection::checkHighPt(Track* /*track*/)
{
  m_flags = static_cast<Flags>(m_flags | HighPt);
}
