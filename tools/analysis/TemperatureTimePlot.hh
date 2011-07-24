#ifndef TemperatureTimePlot_hh
#define TemperatureTimePlot_hh

#include "SensorTimePlot.hh"

#include <QDateTime>

class TemperatureTimePlot : public SensorTimePlot {
public:
  TemperatureTimePlot(SensorTypes::Type, QDateTime first, QDateTime last);
};

#endif
