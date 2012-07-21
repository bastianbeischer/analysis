#ifndef VoltageTimePlot_hh
#define VoltageTimePlot_hh

#include "SensorTimePlot.hh"

#include <QDateTime>

class VoltageTimePlot : public SensorTimePlot {
public:
  VoltageTimePlot(SensorTypes::Type, QDateTime first, QDateTime last);
};

#endif
