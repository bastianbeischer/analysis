#ifndef TrackSelection_hh
#define TrackSelection_hh

#include <QVector>
#include <QPair>

class Track;
class QRectF;

typedef QPair<double, QRectF> Plane;

class TrackSelection
{
  
public:
  TrackSelection();
  ~TrackSelection();
  
  bool passes(Track* track);

private:
  void constructPlanes();

private:
  QVector<Plane> m_planes;
  
};

#endif /* TrackSelection_hh */
