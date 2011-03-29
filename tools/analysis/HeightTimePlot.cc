#include "HeightTimePlot.hh"

#include "SimpleEvent.hh"

#include <TH2.h>
#include <TAxis.h>

#include <cmath>

HeightTimePlot::HeightTimePlot(QDateTime first, QDateTime last)
  : SensorTimePlot(SensorTypes::EBASS_HEIGHT, first, last, 164, -1, 40)
{
  histogram()->GetYaxis()->SetTitle("height / km");
}

void HeightTimePlot::processEvent(const QVector<Hit*>&, Track*, SimpleEvent* event)
{
  double value = event->sensorData(m_type);
  if (!isnan(value))
    histogram()->Fill(event->time(), value / 1000.);
}
