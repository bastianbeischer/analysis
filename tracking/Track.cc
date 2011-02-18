#include "Track.hh"

#include "TrackInformation.hh"
#include "Matrix.hh"
#include "Hit.hh"
#include "Constants.hh"
#include "TOFCluster.hh"
#include "TOFSipmHit.hh"

#include <iostream>
#include <cmath>
#include <cfloat>

#include <QDebug>

Track::Track() :
  m_type(None),
  m_information(new TrackInformation(this)),
  m_matrix(0),
  m_verbose(0),
  m_fitGood(0),
  m_chi2(0),
  m_ndf(0),
  m_pt(0.),
  m_timeOfFlight(0.)
{
}

Track::~Track()
{
  delete m_information;
}

int Track::fit(const QVector<Hit*>& hits)
{
  m_hits = hits;
  m_fitGood = m_matrix->fit(m_hits);
  if (m_fitGood) retrieveFitResults();
  m_information->reset();
  return m_fitGood;
}

void Track::process()
{
  if (m_fitGood) {
    calculatePt();
    calculateTimeOfFlight();
  }
  m_information->process();
}

void Track::calculatePt()
{
  double alpha = bendingAngle();
  if (alpha == 0.)
    m_pt = DBL_MAX;
  else {
    double z0_magnet = -40.; // mm
    double z1_magnet =  40.; // mm
    double x0_magnet = x(z0_magnet); // mm
    double x1_magnet = x(z1_magnet); // mm
    double B_estimate = 0.27; // tesla
    double L  = sqrt(pow(x1_magnet - x0_magnet, 2.) + pow(z1_magnet - z0_magnet,2.))*1000.; // convert from mm to m
    m_pt = 0.3*B_estimate*L/alpha/1e6; // GeV
  }
}

void Track::calculateTimeOfFlight()
{
  QVector<double> allTimes;
  QVector<double> t[2][2];
  foreach (Hit* cluster, m_hits) {
    if (!strcmp(cluster->ClassName(), "TOFCluster")) {
      TOFCluster* tofCluster = static_cast<TOFCluster*>(cluster);
      Q_ASSERT(tofCluster->hits().size() <= 4);
      double z = cluster->position().z();
      if (qAbs(x(z) - tofCluster->position().x()) > 1.2 * Constants::tofBarWidth / 2.)
        continue;
      foreach(Hit* hit, tofCluster->hits()) {
        TOFSipmHit* tofHit = static_cast<TOFSipmHit*>(hit);
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
        allTimes.append(startTime);
      }
    }
  }

  if (t[0][0].size() < 3 || t[0][1].size() < 3 || t[1][0].size() < 3 || t[1][1].size() < 3)
    return;

  qSort(allTimes);
  qSort(t[0][0]);
  qSort(t[0][1]);
  qSort(t[1][0]);
  qSort(t[1][1]);

  double lastTime = lastGoodTime(allTimes);
  double leftStartTime = bestTime(t[0][0], lastTime);
  double rightStartTime = bestTime(t[0][1], lastTime);
  double leftStopTime = bestTime(t[1][0], lastTime);
  double rightStopTime = bestTime(t[1][1], lastTime);

  m_timeOfFlight =
    (leftStopTime < rightStopTime ? leftStopTime : rightStopTime) -
    (leftStartTime < rightStartTime ? leftStartTime : rightStartTime);
  if (qAbs(1./beta()) > 10) {
    qDebug()
      << lastTime << '\n'
      << t[0][0] << " => " << leftStartTime << '\n'
      << t[0][1] << " => " << rightStartTime << '\n'
      << t[1][0] << " => " << leftStopTime << '\n'
      << t[1][1] << " => " << rightStopTime << '\n';
  }
}

double Track::lastGoodTime(QVector<double>& times)
{
  const double dt = 10;
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

double Track::bestTime(QVector<double>& times, double lastTime)
{
  double lgt = lastGoodTime(times);
  int i = times.size() - 1;
  while (i >= 0 && times[i] > qMin(lgt, lastTime)) --i;
  while (i >= 1 && times[i] - times[i-1] < 1) --i;
  if (i < 0)// || !times.size())
    return 0;
  Q_ASSERT(i >= 0 && times.size());
  return times[i];
}

double Track::p() const
{
  double z = 100.;
  double sx = slopeX(z);
  double sy = slopeY(z);
  double p = sqrt((sx*sx + sy*sy + 1)/(sx*sx + 1)) * m_pt;
  return p;
}

double Track::beta() const
{
  return trackLength() / (timeOfFlight() * Constants::speedOfLight);
}
