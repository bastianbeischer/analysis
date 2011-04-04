#include "HeightTimePlot.hh"

#include "SimpleEvent.hh"

#include <TH2.h>
#include <TAxis.h>

#include <math.h>

HeightTimePlot::HeightTimePlot(QDateTime first, QDateTime last)
  : SensorTimePlot(SensorTypes::EBASS_HEIGHT, first, last)
{
  setAxisTitle("time", "height / km");
}

void HeightTimePlot::processEvent(const QVector<Hit*>&, Track*, SimpleEvent* event)
{
  double value = event->sensorData(m_type);
  if (!isnan(value)) {
    double t = event->time();
    histogram()->Fill(t, value / 1000.);
    m_normalizationHistogram->Fill(t);
  }
}
