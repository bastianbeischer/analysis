#include "PressureTimePlot.hh"

#include <TH2.h>
#include <TAxis.h>

PressureTimePlot::PressureTimePlot(SensorTypes::Type type, QDateTime first, QDateTime last)
  : SensorTimePlot(type, first, last)
{
  setAxisTitle("time", "p / mBar");
}
