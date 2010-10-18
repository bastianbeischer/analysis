#ifndef TrackFinding_hh
#define TrackFinding_hh

#include <vector>

class Hit;
class TH2I;
class Track;

class TrackFinding
{
  
public:
  TrackFinding();
  ~TrackFinding();

  std::vector<Hit*> findTrack(std::vector<Hit*> hits);

private:
  bool isInCorridor(Hit* hit) const;

private:
  Track*            m_track;

  double            m_maxSlope;
  double            m_maxOffset;
  TH2I*             m_trackFindingHist;
  
};

#endif /* TrackFinding_hh */
