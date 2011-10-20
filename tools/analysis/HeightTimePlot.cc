#include "HeightTimePlot.hh"

#include "SimpleEvent.hh"

#include <TH2.h>
#include <TAxis.h>

#include <cmath>

HeightTimePlot::HeightTimePlot(QDateTime first, QDateTime last)
  : SensorTimePlot(SensorTypes::EBASS_HEIGHT, first, last)
{
  setAxisTitle("time", "height / km");
}

void HeightTimePlot::processEvent(const AnalyzedEvent* event)
{
  double value = event->simpleEvent()->sensorData(m_type);
  if (!std::isnan(value)) {
    double t = event->simpleEvent()->time();
    histogram()->Fill(t, value / 1000.);
    m_normalizationHistogram->Fill(t);
  }
}
