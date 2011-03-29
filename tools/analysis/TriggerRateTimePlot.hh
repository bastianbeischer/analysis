#ifndef TriggerRateTimePlot_hh
#define TriggerRateTimePlot_hh

#include "SensorTimePlot.hh"

#include <QDateTime>

class TriggerRateTimePlot : public SensorTimePlot {
public:
  TriggerRateTimePlot(QDateTime first, QDateTime last);
};

#endif
