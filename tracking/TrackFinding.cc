#include "TrackFinding.hh"

#include "Hit.hh"
#include "StraightLine.hh"
#include "CenteredBrokenLine.hh"
#include "BrokenLine.hh"

#include <TH2I.h>
#include <cmath>
#include <cassert>

TrackFinding::TrackFinding() :
  m_maxSlope(1.),
  m_maxOffset(100.),
  m_slopeBins(41),
  m_offsetBins(41),
  m_trackerPull(5.),
  m_trdPull(5.),
  m_tofPull(5.),
  m_trackFindingHist(new TH2I("trackFindingHist", "trackFindingHist", m_slopeBins, -m_maxSlope, m_maxSlope, m_offsetBins, -m_maxOffset, m_maxOffset))
{
}

TrackFinding::~TrackFinding()
{
  delete m_trackFindingHist;
}

QVector<Hit*> TrackFinding::findTrack(QVector<Hit*> hits)
{
  m_trackFindingHist->Reset();

  for (QVector<Hit*>::iterator firstHit = hits.begin(); firstHit != hits.end(); firstHit++) {
    for (QVector<Hit*>::iterator secondHit = firstHit; secondHit != hits.end(); secondHit++) {
      if ((*firstHit)->type() == Hit::tof || (*secondHit)->type() == Hit::tof) continue;
      double x1 = (*firstHit)->position().x();
      double x2 = (*secondHit)->position().x();
      double z1 = (*firstHit)->position().z();
      double z2 = (*secondHit)->position().z();

      if (fabs(z2 - z1) > 20) {
        double slope = (x2-x1)/(z2-z1);
        double offset = x1 - slope*z1;
        m_trackFindingHist->Fill(slope,offset);
      }
    }
  }

  int maxX, maxY, maxZ;
  m_trackFindingHist->GetMaximumBin(maxX, maxY, maxZ);
  double slopeMax = m_trackFindingHist->GetXaxis()->GetBinCenter(maxX);
  double offsetMax  = m_trackFindingHist->GetYaxis()->GetBinCenter(maxY);

  StraightLine straightLine;
  straightLine.setSlopeX(slopeMax);
  straightLine.setX0(offsetMax);

  QVector<Hit*> hitsForFit;
  foreach(Hit* hit, hits) {
    if ( (hit->type() == Hit::tracker && isInCorridor(&straightLine, hit, 150)) ||
         (hit->type() == Hit::trd && isInCorridor(&straightLine, hit, 10)) )
      hitsForFit.push_back(hit);
  }

  // improve
  CenteredBrokenLine brokenLine;
  brokenLine.fit(hitsForFit);

  // redetermine hits on track
  QVector<Hit*> hitsOnTrack;
  foreach(Hit* hit, hits) {
    if (isInCorridor(&brokenLine, hit))
      hitsOnTrack.push_back(hit);
  }

  return hitsOnTrack;
}


bool TrackFinding::isInCorridor(Track* track, Hit* hit, double pullOverRide) const
{
  assert(track);

  TVector3 pos = 0.5* (hit->position() + hit->counterPosition());
  TVector3 trackPos = track->position(hit->position().z());

  double angle = hit->angle();
  pos.RotateZ(-angle);
  trackPos.RotateZ(-angle);

  double res = (pos - trackPos).x();
  double resolution = hit->resolutionEstimate();

  double maxPull = 0;
  if (pullOverRide > 0)
    maxPull = pullOverRide;
  else {
    if (hit->type() == Hit::tracker)
      maxPull = m_trackerPull;
    if (hit->type() == Hit::trd)
      maxPull = m_trdPull;
    if (hit->type() == Hit::tof)
      maxPull = m_tofPull;
  }

  return (fabs(res/resolution) < maxPull);
}

