#include "TrackFinding.hh"

#include "Hit.hh"
#include "Track.hh"

#include <TH2I.h>
#include <cmath>
#include <cassert>

TrackFinding::TrackFinding() :
  m_track(0),
  m_maxSlope(1.),
  m_maxOffset(100.),
  m_trackFindingHist(new TH2I("trackFindingHist", "trackFindingHist", 51, -m_maxSlope, m_maxSlope, 51, -m_maxOffset, m_maxOffset))
{
}

TrackFinding::~TrackFinding()
{
  delete m_track;
  delete m_trackFindingHist;
}

QVector<Hit*> TrackFinding::findTrack(QVector<Hit*> hits)
{
  QVector<Hit*> hitsOnTrack;
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

  m_track = new Track;
  m_track->setSlopeX(slopeMax);
  m_track->setX0(offsetMax);

  for (QVector<Hit*>::iterator it = hits.begin(); it != hits.end(); it++) {
    Hit* hit = (*it);
    if (isInCorridor(hit)) hitsOnTrack.push_back(hit);
  }

  //  // improve
  //  m_track->fit(hitsOnTrack);

  // // redetermine hits on track
  // QVector<Hit*> temp = hitsOnTrack;
  // hitsOnTrack.clear();
  // for (QVector<Hit*>::iterator it = temp.begin(); it != temp.end(); it++) {
  //   Hit* hit = (*it);
  //   if (isInCorridor(hit) && hit->type() != Hit::tof) hitsOnTrack.push_back(hit);
  // }

  return hitsOnTrack;
}


bool TrackFinding::isInCorridor(Hit* hit) const
{
  assert(m_track);
  TVector3 pos = hit->position();
  double resolution = hit->resolutionEstimate();
  int maxPull = 10;
  if (hit->type() == Hit::trd)
    maxPull = 5;
  return (fabs(pos.x() - m_track->x(pos.z())) / resolution < maxPull);
}

