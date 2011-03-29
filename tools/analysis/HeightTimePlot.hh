#ifndef HeightTimePlot_hh
#define HeightTimePlot_hh

#include "SensorTimePlot.hh"

#include <QDateTime>

class HeightTimePlot : public SensorTimePlot {
public:
  HeightTimePlot(QDateTime first, QDateTime last);
  virtual void processEvent(const QVector<Hit*>&, Track* = 0, SimpleEvent* = 0);
};

#endif
