#include "MeasurementTimeCalculation.hh"
#include "SimpleEvent.hh"

#include <QDebug>

#include <cmath>

#include <TH1.h>

MeasurementTimeCalculation::MeasurementTimeCalculation()
  : m_histogram(0)
{
}

MeasurementTimeCalculation::MeasurementTimeCalculation(const QDateTime& first, const QDateTime& last)
  : m_histogram(0)
{
  int t1 = first.toTime_t();
  t1-= (t1 % 60) + 60;
  int t2 = last.toTime_t();
  t2+= 120 - (t2 % 60);
  int nBins = (t2 - t1);
  m_histogram = new TH1D("measurement time", "", nBins, t1, t2);
  m_histogram->GetXaxis()->SetTitle("time");
  m_histogram->GetYaxis()->SetTitle("events");

}

MeasurementTimeCalculation::MeasurementTimeCalculation(TH1D* histogram)
  : m_histogram(histogram)
{
}

MeasurementTimeCalculation::~MeasurementTimeCalculation()
{
}

void MeasurementTimeCalculation::update(const SimpleEvent* const event)
{
  if (!event->contentType() == SimpleEvent::MonteCarlo && (event->time() < m_histogram->GetXaxis()->GetXmin() || event->time() > m_histogram->GetXaxis()->GetXmax()))
    qDebug() << "Eventtime is not between first and last" << event->time();
  else
    m_histogram->Fill(event->time());
}

TH1D* MeasurementTimeCalculation::histogram() const
{
  return m_histogram;
}

double MeasurementTimeCalculation::measurementTime() const
{
  const double minEvents = 1;
  double measurementTime = 0;

  double binWidth = m_histogram->GetBinWidth(1);

  for (int bin = 1; bin <= m_histogram->GetNbinsX(); ++bin) {
    double content = histogram()->GetBinContent(bin);
    if (content >= minEvents) {
      measurementTime += binWidth;
    }
  }
  return measurementTime;
}

double MeasurementTimeCalculation::measurementTimeError() const
{
  double binWidth = m_histogram->GetBinWidth(1);
  return binWidth / sqrt(12.);
}
