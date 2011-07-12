#ifndef PressureTimePlot_hh
#define PressureTimePlot_hh

#include "SensorTimePlot.hh"

#include <QDateTime>

class PressureTimePlot : public SensorTimePlot {
public:
  PressureTimePlot(SensorTypes::Type, QDateTime first, QDateTime last);
};

#endif
