#include "TemperatureTimePlot.hh"

#include <TH2.h>
#include <TAxis.h>

TemperatureTimePlot::TemperatureTimePlot(SensorTypes::Type type, QDateTime first, QDateTime last)
  : SensorTimePlot(type, first, last)
{
  histogram()->GetYaxis()->SetTitle("#vartheta / #circC");
}