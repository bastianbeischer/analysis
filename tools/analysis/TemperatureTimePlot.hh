#ifndef TemperatureTimePlot_hh
#define TemperatureTimePlot_hh

#include "AnalysisPlot.hh"
#include "H2DPlot.hh"
#include "SensorTypes.hh"

#include <QDateTime>

class TemperatureTimePlot : public AnalysisPlot, public H2DPlot {
public:
  TemperatureTimePlot(SensorTypes::Type, QDateTime first, QDateTime last);
  virtual ~TemperatureTimePlot();
  virtual void processEvent(const QVector<Hit*>&, Track* = 0, SimpleEvent* = 0);
private:
  SensorTypes::Type m_type;
};

#endif
