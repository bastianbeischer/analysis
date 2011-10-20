#include "TOFBarShiftPlot.hh"
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
#include "Settings.hh"
#include "SimpleEvent.hh"

#include <TH1.h>
#include <TVector3.h>
#include <TLatex.h>
#include <TF1.h>

#include <cmath>

#include <QDebug>

TOFBarShiftPlot::TOFBarShiftPlot(unsigned short idTop1, unsigned short idTop2, unsigned short idBottom1, unsigned short idBottom2)
  : AnalysisPlot(Enums::CalibrationTOF)
  , H1DPlot()
  , m_idTop1(idTop1)
  , m_idTop2(idTop2)
  , m_idBottom1(idBottom1)
  , m_idBottom2(idBottom2)
{
  QString title = QString("bar shift 0x%1 0x%2 0x%3 0x%4")
    .arg(m_idTop1, 0, 16)
    .arg(m_idTop2, 0, 16)
    .arg(m_idBottom1, 0, 16)
    .arg(m_idBottom2, 0, 16);
  setTitle(title);
  TH1D* histogram = new TH1D(qPrintable(title), "", 30, 0, 6);
  setAxisTitle("#Deltat / ns", "");
  addHistogram(histogram);
  TF1* function = new TF1(qPrintable(title + " function"), "gaus(0)", 0, 6);
  function->SetLineColor(kRed);
  addFunction(function);
  addLatex(RootPlot::newLatex(.15, .85));
  addLatex(RootPlot::newLatex(.15, .82));
  addLatex(RootPlot::newLatex(.15, .79));
  addLatex(RootPlot::newLatex(.15, .76));
  addLatex(RootPlot::newLatex(.15, .73));
}

TOFBarShiftPlot::~TOFBarShiftPlot()
{}

void TOFBarShiftPlot::processEvent(const AnalyzedEvent* event)
{
  const Track* track = event->particle()->track();
  if (!track || !track->fitGood())
    return;
  const TimeOfFlight* tof = event->particle()->timeOfFlight();
  const QVector<Hit*>& hits = track->hits();

  const Settings* settings = event->settings();

  ParticleInformation::Flags flags = event->particle()->information()->flags();
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
    unsigned short barId = hit->elementId();
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
        if (event->simpleEvent()->sensorData(SensorTypes::BEAM_CHERENKOV1) > 200 || event->simpleEvent()->sensorData(SensorTypes::BEAM_CHERENKOV2) > 200) {
          m = Constants::electronMass;
        } else {
          m = Constants::pionMass;
        }
      } else {
        if (event->simpleEvent()->sensorData(SensorTypes::BEAM_CHERENKOV1) > 200 || event->simpleEvent()->sensorData(SensorTypes::BEAM_CHERENKOV2) > 200) {
          m = Constants::electronMass;
        } else {
          m = Constants::protonMass; //could also be a pion, doesn't matter
        }
      }
    } else {
      rigidity = track->rigidity();
      m = event->particle()->mass();
    }
    if (rigidity < 1)
      return;
    double t = tof->timeOfFlight();
    double pCorrection = (t + lCorrection) * (1 - sqrt(rigidity*rigidity + m*m) / rigidity);
    double b = Constants::tofBarWidth;
    double xu = track->x(Constants::upperTofPosition);
    double xl = track->x(Constants::lowerTofPosition);
    double upperBarCenter = Constants::tofBarWidth * floor(xu / b) + b / 2.;
    double lowerBarCenter = Constants::tofBarWidth * floor(xl / b) + b / 2.;

    double yu = track->y(Constants::upperTofPosition);
    double yl = track->y(Constants::lowerTofPosition);
    if (qAbs(yu) < 100. && qAbs(yl) < 100. && qAbs(xu-upperBarCenter) < 0.9 * b / 2. && qAbs(xl-lowerBarCenter) < 0.9 * b / 2.)
      histogram()->Fill(t + lCorrection + pCorrection);
  }
}

void TOFBarShiftPlot::update()
{
  latex(0)->SetTitle(qPrintable(QString("n    = %1").arg(histogram(0)->GetEntries())));
  latex(1)->SetTitle(qPrintable(QString("mean = %1 ns").arg(histogram(0)->GetMean(), 0, 'f', 2, ' ')));
  latex(2)->SetTitle(qPrintable(QString("rms  = %1 ns").arg(histogram(0)->GetRMS(), 0, 'f', 2, ' ')));
}

void TOFBarShiftPlot::finalize()
{
  function(0)->SetParameters(histogram(0)->GetMaximum(), histogram(0)->GetMean(), histogram(0)->GetRMS());
  histogram(0)->Fit(function(0), "RQN0");
  latex(3)->SetTitle(qPrintable(QString("t    = %1 ns").arg(function(0)->GetParameter(1), 0, 'f', 2, ' ')));
  latex(4)->SetTitle(qPrintable(QString("#sigma    = %1 ns").arg(function(0)->GetParameter(2), 0, 'f', 2, ' ')));
}
