#include "MeasurementTimeCalculation.hh"

#include "SimpleEvent.hh"
#include "SettingsManager.hh"
#include "Settings.hh"

#include <QDebug>

#include <cmath>

#include <TH1.h>

MeasurementTimeCalculation::MeasurementTimeCalculation()
{
}

MeasurementTimeCalculation::MeasurementTimeCalculation(int numberOfThreads)
  : m_active(numberOfThreads == 1)
  , m_lastEventTime(-1)
  , m_positionInsideBin()
  , m_positionInsideBinCounter()
  , m_timeDifference(0)
  , m_measurementTimeDistribution(0)
  , m_deleteMeasurementTimeDistribution(true)
  , m_situation(Enums::NoSituation)
{
  const int nBins = 60000;
  const double min = 0.;
  const double max = 600.;
  m_positionInsideBin.resize(nBins);
  m_positionInsideBinCounter.resize(nBins);
  QString title = "measurement time distribution";
  m_timeDifference = new TH1D(qPrintable("dt " + title), "", nBins, min, max);
  m_measurementTimeDistribution = new TH1D(qPrintable(title), "", nBins, min, max);
  m_measurementTimeDistribution->GetXaxis()->SetTitle("t_{cut} / s");
  m_measurementTimeDistribution->GetYaxis()->SetTitle("measurement time / s");
  if (!m_active)
    qDebug()
      << "Warning: The measurement time calculation has to be done by only on thread.\n"
      << "A fixed measurement time for the float measurement of 4800.5s and 515390s"
      << "for ground muon measurement (lists/david.txt lists/egon.txt lists/frank.txt)"
      << "will be used for flux calculation.";
}

MeasurementTimeCalculation::MeasurementTimeCalculation(TH1D* histogram)
  : m_active(true)
  , m_timeDifference(0)
  , m_measurementTimeDistribution(histogram)
  , m_deleteMeasurementTimeDistribution(true)
  , m_situation(Enums::NoSituation)
{
}

MeasurementTimeCalculation::~MeasurementTimeCalculation()
{
  delete m_timeDifference;
  if (m_deleteMeasurementTimeDistribution)
    delete m_measurementTimeDistribution;
}

void MeasurementTimeCalculation::update(const SimpleEvent* const event)
{
  const Settings* settings = SettingsManager::instance()->settingsForEvent(event);
  Enums::Situation situation = Enums::NoSituation;
  if (settings)
    situation = settings->situation();
  if (!m_active && m_situation != Enums::NoSituation && m_situation != situation)
    qDebug("Warning: running over files with different settings. For the measurement time a preset value for the float or muon lists is used if more than one thread is active. This leads to a wrong measurement time for the flux calculation");
  m_situation = situation;
  if (!m_active) {
    setFixTime(measurementTime());
    return;
  }
  double eventTime = event->time();
  if (m_lastEventTime > -1) {
    double deltaT = eventTime - m_lastEventTime;
    if (deltaT < 0)
      qDebug("time between events is smaller than 0, check run list order!");
    m_timeDifference->Fill(deltaT);
    int bin = m_timeDifference->FindBin(deltaT);
    if (m_positionInsideBin.size() > bin - 1) {
      m_positionInsideBin[bin-1]+= deltaT;
      ++m_positionInsideBinCounter[bin-1];
    }
  }
  m_lastEventTime = eventTime;
}

double MeasurementTimeCalculation::measurementTime()
{
  if (!m_active) {
    switch (m_situation) {
      case Enums::KirunaFlight:
        return 4800.5;
      case Enums::KirunaMuons:
        return 515390;
      default:
        return 0;
    }
  }
  if (m_timeDifference) {
    double sum = 0;
    for (int cutBin = 1; cutBin <= m_measurementTimeDistribution->GetNbinsX(); ++cutBin) {
      double binContent = m_timeDifference->GetBinContent(cutBin);
      if (binContent > 0)
        sum+= m_timeDifference->GetBinContent(cutBin) * m_positionInsideBin[cutBin-1] / m_positionInsideBinCounter[cutBin-1];
      m_measurementTimeDistribution->SetBinContent(cutBin, sum);
    }
  }
  if (m_measurementTimeDistribution->GetBinContent(0) > 0) {
    return m_measurementTimeDistribution->GetBinContent(0);
  }
  const double tCut = 16.;
  int cutBin = m_measurementTimeDistribution->FindBin(tCut);
  return m_measurementTimeDistribution->GetBinContent(cutBin);
}

TH1D* MeasurementTimeCalculation::measurementTimeDistribution()
{
  m_deleteMeasurementTimeDistribution = false;
  return m_measurementTimeDistribution;
}

void MeasurementTimeCalculation::setFixTime(double time)
{
  m_measurementTimeDistribution->SetBinContent(0, time);
}
