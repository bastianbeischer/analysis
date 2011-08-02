#ifndef TrdTrackFinding_hh
#define TrdTrackFinding_hh

#include "Hit.hh"

#include <TH2D.h>

#include <QVector>
#include <vector>

class TrdTrackFinding {
public:
  TrdTrackFinding();
  virtual ~TrdTrackFinding();

  void apply(const QVector<Hit*>& hits);
  void apply(const std::vector<Hit*>& hits);
  TH2D* histogram() const {return m_slopeVsOffset;}
  double slopeX() const {return m_slopeX;}
  double x() const {return m_x;}
  double entriesAroundMaximum();
  double nHitsOnTrack() const {return m_nHitsOnTrack;}
  bool isInCorridor(Hit* hit, double maxPull);
  bool hasTrdTrack();

private:
  TH2D* m_slopeVsOffset;
  double m_x;
  double m_slopeX;
  int m_nHitsOnTrack;
};

#endif /* TrdTrackFinding_hh */
