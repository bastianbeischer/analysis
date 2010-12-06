#include "TrackFinding.hh"

#include "Hit.hh"
#include "StraightLine.hh"
#include "CenteredBrokenLine.hh"
#include "BrokenLine.hh"

#include <TH2I.h>
#include <cmath>
#include <cassert>
#include <iostream>

TrackFinding::TrackFinding() :
  m_maxSlope(1.),
  m_maxOffset(100.),
  m_slopeBins(41),
  m_offsetBins(41),
  m_trackerPull(30.),
  m_trdPull(5.),
  m_tofPull(5.),
  m_trackFindingHist(new TH2I("trackFindingHist", "trackFindingHist", m_slopeBins, -m_maxSlope, m_maxSlope, m_offsetBins, -m_maxOffset, m_maxOffset))
{
}

TrackFinding::~TrackFinding()
{
  delete m_trackFindingHist;
}

QVector<Hit*> TrackFinding::findTrack(const QVector<Hit*>& hits)
{
  m_trackFindingHist->Reset();

  int max = 0;
  int maxBin = 0;
  for (QVector<Hit*>::ConstIterator firstHit = hits.begin(); firstHit != hits.end(); firstHit++) {
    for (QVector<Hit*>::ConstIterator secondHit = firstHit; secondHit != hits.end(); secondHit++) {
      if ((*firstHit)->type() == Hit::tof || (*secondHit)->type() == Hit::tof) continue;
      double x1 = 0.5*((*firstHit)->position().x() + (*firstHit)->counterPosition().x());
      double x2 = 0.5*((*secondHit)->position().x() + (*secondHit)->counterPosition().x());
      double z1 = (*firstHit)->position().z();
      double z2 = (*secondHit)->position().z();

      if (fabs(z2 - z1) > 20) {
        double slope = (x2-x1)/(z2-z1);
        double offset = x1 - slope*z1;
        int bin = m_trackFindingHist->Fill(slope,offset);
        int content = m_trackFindingHist->GetBinContent(bin);
        if (content > max) {
          max = content;
          maxBin = bin;
        }
      }
    }
  }

  int maxX, maxY, maxZ;
  m_trackFindingHist->GetBinXYZ(maxBin, maxX, maxY, maxZ);
  double slopeMax = m_trackFindingHist->GetXaxis()->GetBinCenter(maxX);
  double offsetMax  = m_trackFindingHist->GetYaxis()->GetBinCenter(maxY);
  StraightLine straightLine;
  straightLine.setSlopeX(slopeMax);
  straightLine.setX0(offsetMax);

  QVector<Hit*> hitsForFit;
  foreach(Hit* hit, hits) {
    if ( (hit->type() == Hit::tracker && isInCorridor(&straightLine, hit, 250)) ||
         (hit->type() == Hit::trd && isInCorridor(&straightLine, hit, 10)) )
      hitsForFit.push_back(hit);
  }

  // improve by redetermining hits on track
  QVector<Hit*> hitsOnTrack;

  CenteredBrokenLine cbl;
  if (cbl.fit(hitsForFit)) {
    foreach(Hit* hit, hits) {
      if (isInCorridor(&cbl, hit))
        hitsOnTrack.push_back(hit);
    }
  }
  else {
    hitsOnTrack = hitsForFit;
  }

  return hitsOnTrack;
}


bool TrackFinding::isInCorridor(Track* track, Hit* hit, double pullOverRide) const
{
  assert(track);

  double z = hit->position().z();
  double hitX = 0.5*(hit->position().x() + hit->counterPosition().x());
  double hitY = 0.5*(hit->position().y() + hit->counterPosition().y());
  double trackX = track->x(z);
  double trackY = track->y(z);

  double angle = (-1.)*hit->angle();
  double c = cos(angle);
  double s = sin(angle);
  double hitU = c*hitX - s*hitY;
  double trackU = c*trackX - s*trackY;

  double res = hitU - trackU;
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

