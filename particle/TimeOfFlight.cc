#include "TimeOfFlight.hh"

#include "Track.hh"
#include "Hit.hh"
#include "Constants.hh"
#include "TOFCluster.hh"
#include "TOFSipmHit.hh"

#include <cfloat>

#include <TMath.h>

#include <QDebug>

TimeOfFlight::TimeOfFlight() :
  m_startTime(0.),
  m_stopTime(0.)
{
}

TimeOfFlight::~TimeOfFlight()
{
}

void TimeOfFlight::reset()
{
  m_startTime = 0.;
  m_stopTime = 0.;
}

void TimeOfFlight::calculateTimes(const Track* track)
{
  QVector<double> startTimes;
  QVector<double> stopTimes;

  const QVector<Hit*>::const_iterator hitsEnd = track->hits().end();
  for (QVector<Hit*>::const_iterator it = track->hits().begin(); it != hitsEnd; ++it) {
    Hit* cluster = *it;
    if (cluster->type() == Hit::tof) {
      TOFCluster* tofCluster = static_cast<TOFCluster*>(cluster);
      Q_ASSERT(tofCluster->hits().size() <= 4);
      double z = cluster->position().z();
      if (qAbs(track->x(z) - tofCluster->position().x()) > 1.2 * Constants::tofBarWidth / 2.)
        continue;
      std::vector<Hit*>& subHits = tofCluster->hits();
      const std::vector<Hit*>::const_iterator subHitsEndIt = subHits.end();
      for (std::vector<Hit*>::const_iterator subHitsIt = subHits.begin(); subHitsIt != subHitsEndIt; ++subHitsIt) {
        TOFSipmHit* tofHit = static_cast<TOFSipmHit*>(*subHitsIt);
        double startTime = tofHit->startTime();
        double tot = tofHit->timeOverThreshold();
        if (startTime < Constants::triggerMatchingMinimum || startTime > Constants::triggerMatchingMaximum || tot < Constants::minimalTotPerSipm)
          continue;
        startTime-= tofHit->photonTravelTime();
        if (z > 0) {
          startTimes.append(startTime);
        } else {
          stopTimes.append(startTime);
        }
      }
    }
  }
  if (startTimes.count() < 6 || stopTimes.count() < 6)
    return;
  qSort(startTimes);
  qSort(stopTimes);
  m_startTime = bestTime(startTimes);
  m_stopTime = bestTime(stopTimes);
}

double TimeOfFlight::bestTime(const QVector<double>& times)
{
  double median = TMath::Median(times.count(), times.constData());
  foreach (double time, times) {
    if (median - time < 0.8) // 1.0ns motivated by TimeReconstructionPlot
      return time;
  }
  return median;
}
