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
#include "SimpleEvent.hh"

#include <TH2.h>
#include <TVector3.h>
#include <TF1.h>
#include <TAxis.h>
#include <TLine.h>

#include <QDebug>
#include <QSpinBox>

TimeResolutionPlot::TimeResolutionPlot(unsigned short id1, unsigned short id2, unsigned short id3, unsigned short id4)
  : AnalysisPlot(Enums::ResolutionTOF)
  , H2DProjectionPlot()
  , m_idTop1(id1)
  , m_idTop2(id2)
  , m_idBottom1(id3)
  , m_idBottom2(id4)
  , m_nBins(10)
{
  QString title = QString("time resolution 0x%1 0x%2, 0x%4 0x%5")
    .arg(m_idTop1, 0, 16).arg(m_idTop2, 0, 16).arg(m_idBottom1, 0, 16).arg(m_idBottom2, 0, 16);
  setTitle(title);
  TH2D* histogram = new TH2D(qPrintable(title), "", m_nBins*m_nBins, 0, m_nBins*m_nBins, 30, 0, 6);
  setAxisTitle("", "#Deltat / ns", "");
  addHistogram(histogram);
  TLine* line = 0;

  double binWidth = Constants::tofBarLength / m_nBins;
  for (int upperBin = 0; upperBin < m_nBins; ++upperBin) {
    double upperCenter = (0.5 + upperBin) * binWidth - Constants::tofBarLength / 2.;
    for (int lowerBin = 0; lowerBin < m_nBins; ++lowerBin) {
      double lowerCenter = (0.5 + lowerBin) * binWidth - Constants::tofBarLength / 2.;
      QString label = QString("u%1 l%2").arg(upperCenter).arg(lowerCenter);
      xAxis()->SetBinLabel(upperBin * m_nBins + lowerBin + 1, qPrintable(label));
    }
  }

  for (int bin = 0; bin < m_nBins - 1; ++bin) {
    line = new TLine();
    line->SetLineColor(kGreen);
    line->SetLineStyle(2);
    line->SetLineWidth(2);
    m_lines.append(line);
  }

}

TimeResolutionPlot::~TimeResolutionPlot()
{}

void TimeResolutionPlot::processEvent(const AnalyzedEvent* event)
{
  const Track* track = event->goodTrack();
  if (!track)
    return;
  const Settings* settings = event->settings();
  if (!event->flagsSet(ParticleInformation::Chi2Good))
    return;
  if (!settings && !event->flagsSet(ParticleInformation::InsideMagnet))
    return;
  if (settings && settings->situation() != Enums::Testbeam11 && !event->flagsSet(ParticleInformation::InsideMagnet))
    return;

  const TimeOfFlight* tof = event->particle()->timeOfFlight();
  const QVector<Hit*>& hits = track->hits();

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
    if (settings && settings->situation() == Enums::Testbeam11) {
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
    double yu = track->y(Constants::upperTofPosition);
    double yl = track->y(Constants::lowerTofPosition);
    double binWidth = Constants::tofBarLength / m_nBins;
    int upperBin = (yu + Constants::tofBarLength / 2.) / binWidth;
    int lowerBin = (yl + Constants::tofBarLength / 2.) / binWidth;
    if (0 <= upperBin && upperBin < m_nBins && 0 <= lowerBin && lowerBin < m_nBins)
      histogram()->Fill(upperBin * m_nBins + lowerBin, t + lCorrection + pCorrection);
  }
}

void TimeResolutionPlot::update()
{
  for (int bin = 0; bin < m_nBins - 1; ++bin) {
    m_lines[bin]->SetX1((bin + 1) * m_nBins);
    m_lines[bin]->SetX2((bin + 1) * m_nBins);
    m_lines[bin]->SetY1(0);
    m_lines[bin]->SetY2(yAxis()->GetXmax());
  }
  AnalysisPlot::update();
}

void TimeResolutionPlot::draw(TCanvas* canvas)
{
  H2DPlot::draw(canvas);
  for (int bin = 0; bin < m_nBins - 1; ++bin)
    m_lines[bin]->Draw();
}
