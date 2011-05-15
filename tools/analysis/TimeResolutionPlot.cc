#include "TimeResolutionPlot.hh"
#include "BrokenLine.hh"
#include "ParticleInformation.hh"
#include "Hit.hh"
#include "Constants.hh"
#include "TOFCluster.hh"
#include "TOFSipmHit.hh"
#include "Setup.hh"
#include "TOFBar.hh"
#include "Particle.hh"
#include "Track.hh"
#include "TimeOfFlight.hh"
#include "ProjectionControlWidget.hh"
#include "Settings.hh"
#include "SettingsManager.hh"
#include "SimpleEvent.hh"

#include <TH2.h>
#include <TVector3.h>
#include <TLatex.h>
#include <TF1.h>

#include <QDebug>
#include <QSpinBox>

TimeResolutionPlot::TimeResolutionPlot(unsigned short idTop1, unsigned short idTop2, unsigned short idBottom1, unsigned short idBottom2)
  : AnalysisPlot(AnalysisPlot::ResolutionTOF)
  , H2DProjectionPlot()
  , m_idTop1(idTop1)
  , m_idTop2(idTop2)
  , m_idBottom1(idBottom1)
  , m_idBottom2(idBottom2)
{
  controlWidget()->spinBox()->setMaximum(64);
  QString title = QString("time resolution 0x%1 0x%2 0x%3 0x%4")
    .arg(m_idTop1, 0, 16)
    .arg(m_idTop2, 0, 16)
    .arg(m_idBottom1, 0, 16)
    .arg(m_idBottom2, 0, 16);
  setTitle(title);
  TH2D* histogram = new TH2D(qPrintable(title), "",
    5, -Constants::tofBarLength / 2., Constants::tofBarLength / 2., 30, 0, 6);
  setAxisTitle("y / mm", "#Deltat / ns", "");
  addHistogram(histogram);
}

TimeResolutionPlot::~TimeResolutionPlot()
{}

void TimeResolutionPlot::processEvent(const QVector<Hit*>&, Particle* particle, SimpleEvent* event)
{
  const Track* track = particle->track();
  if (!track || !track->fitGood())
    return;
  const TimeOfFlight* tof = particle->timeOfFlight();
  const QVector<Hit*>& hits = track->hits();

  const Settings* settings = SettingsManager::instance()->settingsForEvent(event);

  ParticleInformation::Flags flags = particle->information()->flags();
  if (!(flags & ParticleInformation::Chi2Good))
    return;
  if (!settings && !(flags & ParticleInformation::InsideMagnet))
    return;
  if (settings && settings->situation() != Settings::Testbeam11 && !(flags & ParticleInformation::InsideMagnet))
    return;
  bool idTop1 = false, idTop2 = false, idBottom1 = false, idBottom2 = false;
  const QVector<Hit*>::const_iterator endIt = hits.end();
  for (QVector<Hit*>::const_iterator it = hits.begin(); it != endIt; ++it) {
    Hit* hit = *it;
    unsigned short barId = hit->detId() - hit->channel();
    if (!idTop1 && barId == m_idTop1) idTop1 = true;
    if (!idTop2 && barId == m_idTop2) idTop2 = true;
    if (!idBottom1 && barId == m_idBottom1) idBottom1 = true;
    if (!idBottom2 && barId  == m_idBottom2) idBottom2 = true;
  }
  if (idTop1 && idTop2 && idBottom1 && idBottom2) {
    double l = track->trackLength();
    double d = Constants::upperTofPosition - Constants::lowerTofPosition;
    double lCorrection = (d - l) / Constants::speedOfLight;
    double rigidity = 0;
    double m = 0;
    if (settings && settings->situation() == Settings::Testbeam11) {
      rigidity = settings->momentum();
      if (settings->polarity() < 0) {
        if (event->sensorData(SensorTypes::BEAM_CHERENKOV1) > 200 || event->sensorData(SensorTypes::BEAM_CHERENKOV2) > 200) {
          m = Constants::electronMass;
        } else {
          m = Constants::pionMass;
        }
      } else {
        if (event->sensorData(SensorTypes::BEAM_CHERENKOV1) > 200 || event->sensorData(SensorTypes::BEAM_CHERENKOV2) > 200) {
          m = Constants::electronMass;
        } else {
          m = Constants::protonMass; //could also be a pion, doesn't matter
        }
      }
    } else {
      rigidity = track->rigidity();
      m = particle->mass();
    }
    if (rigidity < 1)
      return;
    double t = tof->timeOfFlight();
    double pCorrection = (t + lCorrection) * (1 - sqrt(rigidity*rigidity + m*m) / rigidity);
    double yu = track->y(Constants::upperTofPosition);
    double yl = track->y(Constants::lowerTofPosition);
    double binWidth = xAxis()->GetBinWidth(1);
    if (qAbs(yu - yl) < binWidth)
      histogram()->Fill((yu + yl) / 2., t + lCorrection + pCorrection);
  }
}
