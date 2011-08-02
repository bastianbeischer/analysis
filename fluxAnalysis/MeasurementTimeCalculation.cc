#include "MeasurementTimeCalculation.hh"

#include <QDebug>

#include <cmath>

MeasurementTimeCalculation::MeasurementTimeCalculation()
{
  m_measurementTimeError = 0;
  m_measurementTime = 0;
}

MeasurementTimeCalculation::MeasurementTimeCalculation(QDateTime first, QDateTime last) :
  m_measurementTime(0)
{
  int t1 = first.toTime_t();
  t1-= (t1 % 60) + 60;
  int t2 = last.toTime_t();
  t2+= 120 - (t2 % 60);
  int nBins = (t2 - t1);
  m_histogram = new TH1D("measurement time", "", nBins, t1, t2);
  m_histogram->GetXaxis()->SetTitle("time");
  m_histogram->GetYaxis()->SetTitle("events");

  double binWidth = m_histogram->GetBinWidth(1);
  m_measurementTimeError = binWidth / sqrt(12);
}

MeasurementTimeCalculation::MeasurementTimeCalculation(TH1D* histogram) :
  m_histogram(histogram)
{

}

MeasurementTimeCalculation::~MeasurementTimeCalculation()
{
  if (m_histogram) {
    delete m_histogram;
    m_histogram = 0;
  }
}

void MeasurementTimeCalculation::update(const SimpleEvent* const event)
{
  m_mutex.lock();
  if (!event->contentType() == SimpleEvent::MonteCarlo && (event->time() < m_histogram->GetXaxis()->GetXmin() || event->time() > m_histogram->GetXaxis()->GetXmax()))
    qDebug() << "Eventtime is not between first and last" << event->time();
  else
    m_histogram->Fill(event->time());
  m_mutex.unlock();
}

TH1D* MeasurementTimeCalculation::histogram()
{
  return m_histogram;
}

double MeasurementTimeCalculation::measurementTime()
{
  const double minEvents = 1;
  double measurementTime = 0;
  m_mutex.lock();

  double binWidth = m_histogram->GetBinWidth(1);

  for (int bin = 1; bin <= m_histogram->GetNbinsX(); ++bin) {
    double content = histogram()->GetBinContent(bin);
    if (content >= minEvents) {
      measurementTime += binWidth;
    }
  }
  m_measurementTime = measurementTime;
  m_mutex.unlock();
  return m_measurementTime;
}

double MeasurementTimeCalculation::measurementTimeError()
{
  double binWidth = m_histogram->GetBinWidth(1);
  m_measurementTimeError = binWidth / sqrt(12);

  return m_measurementTimeError;
}
