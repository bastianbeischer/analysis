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
  virtual ~TrackFinding();

  void setSlopeBins(int nBins) {m_slopeBins = nBins;}
  void setOffsetBins(int nBins) {m_offsetBins = nBins;}
  void setTrackerPull(double pull) {m_trackerPull = pull;}
  void setTrdPull(double pull) {m_trdPull = pull;}
  void setTofPull(double pull) {m_tofPull = pull;}

  QVector<Hit*> findTrack(const QVector<Hit*>& hits);

private:
  bool isInCorridor(Track* track, Hit* hit, double pullOverRide = -1) const;

private:
  double            m_maxSlope;
  double            m_maxOffset;
  int               m_slopeBins;
  int               m_offsetBins;

  double            m_trackerPull;
  double            m_trdPull;
  double            m_tofPull;

  TH2I*             m_trackFindingHist;
  static int        s_histCounter;
  
};

#endif /* TrackFinding_hh */
