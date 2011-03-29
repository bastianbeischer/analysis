#include "TriggerRateTimePlot.hh"

#include <TH2.h>
#include <TAxis.h>

TriggerRateTimePlot::TriggerRateTimePlot(QDateTime first, QDateTime last)
  : SensorTimePlot(SensorTypes::TRIGGER_RATE, first, last, 208, -1, 51)
{
  histogram()->GetYaxis()->SetTitle("trigger rate / Hz");
}
