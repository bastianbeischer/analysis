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
  double t[4][4];
  for (int i = 0; i < 4; ++i)
    for (int j = 0; j < 4; ++j)
      t[i][j] = -2e10;

  foreach (Hit* hit, m_hits) {
    if (!strcmp(hit->ClassName(), "TOFCluster")) {
      TOFCluster* cluster = static_cast<TOFCluster*>(hit);
      Q_ASSERT(cluster->hits().size() <= 4);

      // if cluster is on track, TODO: provide some contingency because the cluster->position().x() is not at the nominal place
      if (qAbs(x(hit->position().z()) - cluster->position().x()) <= Constants::tofBarWidth / 2.) {
        int layer = -1;
        double z = cluster->position().z();
        if (Constants::upperTofPosition < z && z < Constants::upperTofPosition + Constants::tofBarHeight)
          layer = 0;
        else if (Constants::upperTofPosition - Constants::tofBarHeight < z && z < Constants::upperTofPosition)
          layer = 1;
        else if (Constants::lowerTofPosition < z && z < Constants::lowerTofPosition + Constants::tofBarHeight)
          layer = 2;
        else if (Constants::lowerTofPosition - Constants::tofBarHeight < z && z < Constants::lowerTofPosition)
          layer = 3;

        Q_ASSERT(layer > -1);
        for (unsigned int i = 0; i < cluster->hits().size(); ++i) {
          TOFSipmHit* tofHit = static_cast<TOFSipmHit*>(cluster->hits()[i]);
          if (tofHit->position().y() < 0) {
            if (tofHit->position().x() < cluster->position().x()) {
              Q_ASSERT(t[layer][0] < -1e10);
              t[layer][0] = tofHit->startTime() - tofHit->photonTravelTime();
            }
            if (tofHit->position().x() > cluster->position().x()) {
              Q_ASSERT(t[layer][1] < -1e10);
              t[layer][1] = tofHit->startTime() - tofHit->photonTravelTime();
            }
          }
          if (tofHit->position().y() > 0) {
            if (tofHit->position().x() < cluster->position().x()) {
              Q_ASSERT(t[layer][2] < -1e10);
              t[layer][2] = tofHit->startTime() - tofHit->photonTravelTime();
            }
            if (tofHit->position().x() > cluster->position().x()) {
              Q_ASSERT(t[layer][3] < -1e10);
              t[layer][3] = tofHit->startTime() - tofHit->photonTravelTime();
            }
          }
        }
      }
    }
  }

  QVector<double> leftStartTimes;
  if (t[0][0] >= -1e10) leftStartTimes.append(t[0][0]);
  if (t[0][1] >= -1e10) leftStartTimes.append(t[0][1]);
  if (t[1][0] >= -1e10) leftStartTimes.append(t[1][0]);
  if (t[1][1] >= -1e10) leftStartTimes.append(t[1][1]);
  double leftStartTime = bestTime(leftStartTimes);

  QVector<double> rightStartTimes;
  if (t[0][2] >= -1e10) rightStartTimes.append(t[0][2]);
  if (t[0][3] >= -1e10) rightStartTimes.append(t[0][3]);
  if (t[1][2] >= -1e10) rightStartTimes.append(t[1][2]);
  if (t[1][3] >= -1e10) rightStartTimes.append(t[1][3]);
  double rightStartTime = bestTime(rightStartTimes);

  QVector<double> leftStopTimes;
  if (t[2][0] >= -1e10) leftStopTimes.append(t[2][0]);
  if (t[2][1] >= -1e10) leftStopTimes.append(t[2][1]);
  if (t[3][0] >= -1e10) leftStopTimes.append(t[3][0]);
  if (t[3][1] >= -1e10) leftStopTimes.append(t[3][1]);
  double leftStopTime = bestTime(leftStopTimes);

  QVector<double> rightStopTimes;
  if (t[2][2] >= -1e10) rightStopTimes.append(t[2][2]);
  if (t[2][3] >= -1e10) rightStopTimes.append(t[2][3]);
  if (t[3][2] >= -1e10) rightStopTimes.append(t[3][2]);
  if (t[3][3] >= -1e10) rightStopTimes.append(t[3][3]);
  double rightStopTime = bestTime(rightStopTimes);

  m_timeOfFlight =
    (leftStopTime < rightStopTime ? leftStopTime : rightStopTime) -
    (leftStartTime < rightStartTime ? leftStartTime : rightStartTime);
}

double Track::bestTime(QVector<double>& t)
{
  QVector<double> times = t;
  qSort(times);
  for (int i = 1; i <= 2 && i < times.size();) {
    if (times[i] - times[i-1] > 7.5)
      times.remove(i-1);
    else {
      ++i;
    }
  }
  return times.size() ? times[0] : 0;
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
