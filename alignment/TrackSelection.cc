#include "TrackSelection.hh"

#include "Track.hh"

TrackSelection::TrackSelection()
{
}

TrackSelection::~TrackSelection()
{
}

bool TrackSelection::passes(Track* /*track*/)
{
  return true;
}
