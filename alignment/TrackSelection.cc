#include "TrackSelection.hh"

#include "Track.hh"
#include "Hit.hh"

#include <QRectF>

TrackSelection::TrackSelection()
{
  constructPlanes();
}

TrackSelection::~TrackSelection()
{
}

void TrackSelection::constructPlanes()
{
  double width = 150.;
  double height = 200.;
  QRectF rect(QPointF(-width/2., -height/2.), QPointF(width/2., height/2.));

  // upper magnet boundary
  m_planes.push_back(Plane( 100., rect));
  // lower magnet boundary
  m_planes.push_back(Plane(-100., rect));
}

bool TrackSelection::passes(Track* track)
{
  bool passes = true;

  QVector<Hit*> hits = track->hits();
  int nTrackerHits = 0;
  foreach(Hit* hit, hits)
    if (hit->type() == Hit::tracker)
      nTrackerHits++;

  if (nTrackerHits != 8)
    passes = false;

  foreach(Plane plane, m_planes) {
    double z = plane.first;
    double x = track->x(z);
    double y = track->y(z);

    QRectF rectangle = plane.second;
    // if (rectangle.contains(x,y))
    //   passes = false;
                   
  }
  return passes;
}
