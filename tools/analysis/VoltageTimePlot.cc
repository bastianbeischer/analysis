#include "VoltageTimePlot.hh"

#include <TH2.h>
#include <TAxis.h>

VoltageTimePlot::VoltageTimePlot(SensorTypes::Type type, QDateTime first, QDateTime last)
  : SensorTimePlot(type, first, last)
{
  setAxisTitle("time", "U / V");
}
