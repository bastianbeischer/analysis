#include "TimeOfFlightPlot.hh"
#include "BrokenLine.hh"
#include "TrackSelection.hh"
#include "Hit.hh"
#include "TOFConstants.hh"
#include "TOFCluster.hh"
#include "TOFSipmHit.hh"
#include "Setup.hh"
#include "TOFBar.hh"

#include <TH1.h>
#include <TVector3.h>

#include <QDebug>

TimeOfFlightPlot::TimeOfFlightPlot()
  : AnalysisPlot(AnalysisPlot::MiscellaneousTOF)
  , H1DPlot()
{
  setTitle("time of flight");
  TH1D* histogram = new TH1D("time of flight", "", 1200, -60, 60);
  histogram->GetXaxis()->SetTitle("c #Deltat / L_{track}");
  addHistogram(histogram);
}

TimeOfFlightPlot::~TimeOfFlightPlot()
{}

void TimeOfFlightPlot::processEvent(const QVector<Hit*>& hits, Track* track, TrackSelection* selection, SimpleEvent*)
{
  // QMutexLocker locker(&m_mutex);
  if (!track || !selection || !track->fitGood())
    return;

  TrackSelection::Flags flags = selection->flags();
  if (!(flags & TrackSelection::AllTrackerLayers))
    return;

  double t[4][4] = {{-1}};

  foreach (Hit* hit, hits) {
    if (!strcmp(hit->ClassName(), "TOFCluster")) {
      TOFCluster* cluster = static_cast<TOFCluster*>(hit);
      Q_ASSERT(cluster->hits().size() <= 4);

      TOFBar* element = static_cast<TOFBar*>(Setup::instance()->element(cluster->detId()));
      //qDebug() << element->timeShifts() << hex << element->id();

      // if cluster is on track
      if (qAbs(track->x(hit->position().z()) - cluster->position().x()) <= tofBarWidth / 2.) {
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
              Q_ASSERT(t[layer][0] == 0);
              t[layer][0] = tofHit->startTime();
            }
            if (tofHit->position().x() > cluster->position().x()) {
              Q_ASSERT(t[layer][1] == 0);
              t[layer][1] = tofHit->startTime();
            }
          }
          if (tofHit->position().y() > 0) {
            if (tofHit->position().x() < cluster->position().x()) {
              Q_ASSERT(t[layer][2] == 0);
              t[layer][2] = tofHit->startTime();
            }
            if (tofHit->position().x() > cluster->position().x()) {
              Q_ASSERT(t[layer][3] == 0);
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

  const TVector3& upperPoint = track->position(upperTofPosition);
  const TVector3& lowerPoint = track->position(lowerTofPosition);
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
  double deltaT = (correctedLeftStopTime+correctedRightStopTime)/2. - (correctedLeftStartTime+correctedRightStartTime)/2.;
  double trackLength = (upperPoint-lowerPoint).Mag();
  histogram(0)->Fill(deltaT * speedOfLight / trackLength);
}

void TimeOfFlightPlot::finalize()
{
}
