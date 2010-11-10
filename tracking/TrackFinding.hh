#ifndef TrackFinding_hh
#define TrackFinding_hh

#include <QVector>

class Hit;
class TH2I;
class Track;

class TrackFinding
{
  
public:
  TrackFinding();
  ~TrackFinding();

  QVector<Hit*> findTrack(QVector<Hit*> hits);

private:
  bool isInCorridor(Track* track, Hit* hit, int maxPull = 5) const;

private:
  double            m_maxSlope;
  double            m_maxOffset;
  TH2I*             m_trackFindingHist;
  
};

#endif /* TrackFinding_hh */
