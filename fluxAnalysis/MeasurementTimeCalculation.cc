#include "MeasurementTimeCalculation.hh"
#include "SimpleEvent.hh"

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
    qDebug("Warning: The measurement time calculation has to be done by only on thread.");
}

MeasurementTimeCalculation::MeasurementTimeCalculation(TH1D* histogram)
: m_measurementTimeDistribution(histogram)
, m_deleteMeasurementTimeDistribution(true)
, m_active(true)
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
  if (!m_active)
    return;
  double eventTime = event->time();
  if (m_lastEventTime > -1) {
    double deltaT = eventTime - m_lastEventTime;
    if (deltaT < 0)
      qDebug("time between events is smaller than 0, check run list order!");
    m_timeDifference->Fill(deltaT);
    int bin = m_timeDifference->FindBin(deltaT);
    m_positionInsideBin[bin-1]+= deltaT;
    ++m_positionInsideBinCounter[bin-1];
  }
  m_lastEventTime = eventTime;
}

double MeasurementTimeCalculation::measurementTime()
{
  if (!m_active)
    return 4800.5;
  if (m_timeDifference) {
    double sum = 0;
    for (int cutBin = 1; cutBin <= m_measurementTimeDistribution->GetNbinsX(); ++cutBin) {
      double binContent = m_timeDifference->GetBinContent(cutBin);
      if (binContent > 0)
        sum+= m_timeDifference->GetBinContent(cutBin) * m_positionInsideBin[cutBin-1] / m_positionInsideBinCounter[cutBin-1];
      m_measurementTimeDistribution->SetBinContent(cutBin, sum);
    }
  }
  const double tCut = 10.;
  int cutBin = m_measurementTimeDistribution->FindBin(tCut);
  return m_measurementTimeDistribution->GetBinContent(cutBin);
}

TH1D* MeasurementTimeCalculation::measurementTimeDistribution()
{
  m_deleteMeasurementTimeDistribution = false;
  return m_measurementTimeDistribution;
}
