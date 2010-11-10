#ifndef TrackFinding_hh
#define TrackFinding_hh

#include <QVector>

class Hit;
class TH2I;
class StraightLine;

class TrackFinding
{
  
public:
  TrackFinding();
  ~TrackFinding();

  QVector<Hit*> findTrack(QVector<Hit*> hits);

private:
  bool isInCorridor(Hit* hit) const;

private:
  StraightLine*     m_track;

  double            m_maxSlope;
  double            m_maxOffset;
  TH2I*             m_trackFindingHist;
  
};

#endif /* TrackFinding_hh */
