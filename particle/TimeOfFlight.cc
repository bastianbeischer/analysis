#include "TimeOfFlight.hh"

#include "Track.hh"
#include "Hit.hh"
#include "Constants.hh"
#include "TOFCluster.hh"
#include "TOFSipmHit.hh"

#include <cfloat>

#include <TMath.h>

TimeOfFlight::TimeOfFlight() :
  m_startTime(0.),
  m_stopTime(0.)
{
}

TimeOfFlight::~TimeOfFlight()
{
}

void TimeOfFlight::calculateTimes(const Track* track)
{
  QVector<double> t[2][2];
  QVector<double> startTimes;
  QVector<double> stopTimes;

  const QVector<Hit*>::const_iterator hitsEnd = track->hits().end();
  for (QVector<Hit*>::const_iterator it = track->hits().begin(); it != hitsEnd; ++it) {
    Hit* cluster = *it;
    if (!strcmp(cluster->ClassName(), "TOFCluster")) {
      TOFCluster* tofCluster = static_cast<TOFCluster*>(cluster);
      Q_ASSERT(tofCluster->hits().size() <= 4);
      double z = cluster->position().z();
      if (qAbs(track->x(z) - tofCluster->position().x()) > 1.2 * Constants::tofBarWidth / 2.)
        continue;
      std::vector<Hit*>& subHits = tofCluster->hits();
      const std::vector<Hit*>::const_iterator subHitsEndIt = subHits.end();
      for (std::vector<Hit*>::const_iterator subHitsIt = subHits.begin(); subHitsIt != subHitsEndIt; ++subHitsIt) {
        TOFSipmHit* tofHit = static_cast<TOFSipmHit*>(*subHitsIt);
        double startTime = tofHit->startTime() - tofHit->photonTravelTime();
        if (z > 0) {
          if (tofHit->position().y() < 0)
            t[0][0].append(startTime);
          else
            t[0][1].append(startTime);
        } else {
          if (tofHit->position().y() < 0)
            t[1][0].append(startTime);
          else
            t[1][1].append(startTime);
        }
      }
    }
  }

  if (t[0][0].size() < 3 || t[0][1].size() < 3 || t[1][0].size() < 3 || t[1][1].size() < 3)
    return;

  qSort(t[0][0]);
  qSort(t[0][1]);
  qSort(t[1][0]);
  qSort(t[1][1]);

  double leftStartTime = bestTime(t[0][0]);
  double rightStartTime = bestTime(t[0][1]);
  double leftStopTime = bestTime(t[1][0]);
  double rightStopTime = bestTime(t[1][1]);

  double* values = new double[4];
  values[0] = leftStartTime;
  values[1] = rightStartTime;
  values[2] = leftStopTime;
  values[3] = rightStopTime;
  double median = TMath::Median(4, values);
  delete[] values;
  if (qAbs(leftStartTime - median) > 10) leftStartTime = DBL_MAX;
  if (qAbs(rightStartTime - median) > 10) rightStartTime = DBL_MAX;
  if (qAbs(leftStopTime - median) > 10) leftStopTime = DBL_MAX;
  if (qAbs(rightStopTime - median) > 10) rightStopTime = DBL_MAX;

  m_startTime = (leftStartTime < rightStartTime ? leftStartTime : rightStartTime);
  m_stopTime = (leftStopTime < rightStopTime ? leftStopTime : rightStopTime);
}

double TimeOfFlight::lastGoodTime(QVector<double>& times)
{
  const double dt = 5;
  int n = 0;
  
  QVector<int> first;
  QVector<int> last;
  n = times.size();
  Q_ASSERT(n);
  for (int i = 0; i < n-1; ++i) {
    if (times[i+1] - times[i] < dt && last.size() == first.size())
      first.append(i);
    if (times[i+1] - times[i] > dt && last.size() < first.size())
      last.append(i);
  }
  if (last.size() < first.size())
    last.append(n-1);
  Q_ASSERT(first.size() == last.size());

  int max = 0;
  n = first.size();
  if (n <= 0)
    return DBL_MAX;
  Q_ASSERT(n > 0);
  for (int i = 0; i < n; ++i)
    if (last[i] - first[i] > last[max] - first[max])
      max = i;

  return times[last[max]];
}

double TimeOfFlight::bestTime(QVector<double>& times)
{
  double lastTime = lastGoodTime(times);
  int i = times.size() - 1;
  while (i >= 0 && times[i] > lastTime) --i;
  while (i >= 1 && times[i] - times[i-1] < 5) --i;
  //if (i < 0)
    //return DBL_MAX;
  Q_ASSERT(i >= 0 && times.size());
  return times[i];
}
