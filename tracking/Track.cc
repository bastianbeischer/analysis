#include "Track.hh"

#include "TrackInformation.hh"
#include "Matrix.hh"
#include "Hit.hh"
#include "TOFConstants.hh"
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

int Track::process(const QVector<Hit*>& hits)
{
  int retVal = fit(hits);
  calculatePt();
  calculateTimeOfFlight();
  return retVal;
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
      t[i][j] = -1;

  foreach (Hit* hit, m_hits) {
    if (!strcmp(hit->ClassName(), "TOFCluster")) {
      TOFCluster* cluster = static_cast<TOFCluster*>(hit);
      Q_ASSERT(cluster->hits().size() <= 4);

      // if cluster is on track
      if (qAbs(x(hit->position().z()) - cluster->position().x()) <= tofBarWidth / 2.) {
        int layer = -1;
        double z = cluster->position().z();
        if (upperTofPosition < z && z < upperTofPosition + tofBarHeight)
          layer = 0;
        else if (upperTofPosition - tofBarHeight < z && z < upperTofPosition)
          layer = 1;
        else if (lowerTofPosition < z && z < lowerTofPosition + tofBarHeight)
          layer = 2;
        else if (lowerTofPosition - tofBarHeight < z && z < lowerTofPosition)
          layer = 3;

        Q_ASSERT(layer > -1);
        //qDebug() << cluster->hits().size();
        for (unsigned int i = 0; i < cluster->hits().size(); ++i) {
          TOFSipmHit* tofHit = static_cast<TOFSipmHit*>(cluster->hits()[i]);
          if (tofHit->position().y() < 0) {
            if (tofHit->position().x() < cluster->position().x()) {
              Q_ASSERT(t[layer][0] < 0);
              t[layer][0] = tofHit->startTime();
            }
            if (tofHit->position().x() > cluster->position().x()) {
              Q_ASSERT(t[layer][1] < 0);
              t[layer][1] = tofHit->startTime();
            }
          }
          if (tofHit->position().y() > 0) {
            if (tofHit->position().x() < cluster->position().x()) {
              Q_ASSERT(t[layer][2] < 0);
              t[layer][2] = tofHit->startTime();
            }
            if (tofHit->position().x() > cluster->position().x()) {
              Q_ASSERT(t[layer][3] < 0);
              t[layer][3] = tofHit->startTime();
            }
          }
        }
      }
    }
  }

  QVector<double> leftStartTimes;
  if (t[0][0] >= 0) leftStartTimes.append(t[0][0]);
  if (t[0][1] >= 0) leftStartTimes.append(t[0][1]);
  if (t[1][0] >= 0) leftStartTimes.append(t[1][0]);
  if (t[1][1] >= 0) leftStartTimes.append(t[1][1]);
  double leftStartTime = 0;
  foreach (double time, leftStartTimes)
    leftStartTime+= time;
  leftStartTime/= leftStartTimes.size();

  QVector<double> rightStartTimes;
  if (t[0][2] >= 0) rightStartTimes.append(t[0][2]);
  if (t[0][3] >= 0) rightStartTimes.append(t[0][3]);
  if (t[1][2] >= 0) rightStartTimes.append(t[1][2]);
  if (t[1][3] >= 0) rightStartTimes.append(t[1][3]);
  double rightStartTime = 0;
  foreach (double time, rightStartTimes)
    rightStartTime+= time;
  rightStartTime/= rightStartTimes.size();

  QVector<double> leftStopTimes;
  if (t[2][0] >= 0) leftStopTimes.append(t[2][0]);
  if (t[2][1] >= 0) leftStopTimes.append(t[2][1]);
  if (t[3][0] >= 0) leftStopTimes.append(t[3][0]);
  if (t[3][1] >= 0) leftStopTimes.append(t[3][1]);
  double leftStopTime = 0;
  foreach (double time, leftStopTimes)
    leftStopTime+= time;
  leftStopTime/= leftStopTimes.size();

  QVector<double> rightStopTimes;
  if (t[2][2] >= 0) rightStopTimes.append(t[2][2]);
  if (t[2][3] >= 0) rightStopTimes.append(t[2][3]);
  if (t[3][2] >= 0) rightStopTimes.append(t[3][2]);
  if (t[3][3] >= 0) rightStopTimes.append(t[3][3]);
  double rightStopTime = 0;
  foreach (double time, rightStopTimes)
    rightStopTime+= time;
  rightStopTime/= rightStopTimes.size();

  const TVector3& upperPoint = position(upperTofPosition);
  const TVector3& lowerPoint = position(lowerTofPosition);
  double distanceToLeftStartSipm = tofBarLength/2. + upperPoint.y();
  double correctedLeftStartTime = leftStartTime - distanceToLeftStartSipm * tofRefractiveIndex / speedOfLight;
  double distanceToRightStartSipm = tofBarLength/2. - upperPoint.y();
  double correctedRightStartTime = rightStartTime - distanceToRightStartSipm * tofRefractiveIndex / speedOfLight;
  double distanceToLeftStopSipm = tofBarLength/2. + lowerPoint.y(); 
  double correctedLeftStopTime = leftStopTime - distanceToLeftStopSipm * tofRefractiveIndex / speedOfLight;
  double distacneToRightStopSipm = tofBarLength/2. - lowerPoint.y();
  double correctedRightStopTime = rightStopTime - distacneToRightStopSipm * tofRefractiveIndex / speedOfLight;

  //qDebug() << leftStartTimes.size() << rightStartTimes.size() << leftStopTimes.size() << rightStopTimes.size();
  //qDebug() << distanceToLeftStartSipm << distanceToRightStartSipm << distanceToLeftStopSipm << distacneToRightStopSipm;
  //qDebug() << leftStartTime << rightStartTime << leftStopTime << rightStopTime;
  //qDebug() << correctedLeftStartTime << correctedRightStartTime << correctedLeftStopTime << correctedRightStopTime;
  //qDebug() << "---";
  m_timeOfFlight = (correctedLeftStopTime+correctedRightStopTime)/2. - (correctedLeftStartTime+correctedRightStartTime)/2.;
}

double Track::p() const
{
  double z = 100.;
  double sx = slopeX(z);
  double sy = slopeY(z);
  double p = sqrt((sx*sx + sy*sy + 1)/(sx*sx + 1)) * m_pt;
  return p;
}
