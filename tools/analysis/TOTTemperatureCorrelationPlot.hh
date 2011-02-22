#ifndef TOTTemperatureCorrelationPlot_hh
#define TOTTemperatureCorrelationPlot_hh

#include "AnalysisPlot.hh"
#include "H2DPlot.hh"
#include "SensorTypes.hh"
#include "TOFSensorTypeAssignment.hh"

#include <QVector>
#include <QMap>

class Hit;
class Track;
class SimpleEvent;

class TOTTemperatureCorrelationPlot : public AnalysisPlot, public H2DPlot {
public:
  TOTTemperatureCorrelationPlot(unsigned int tofChannel);
  ~TOTTemperatureCorrelationPlot();
  
  virtual void processEvent(const QVector<Hit*>&, Track* = 0, SimpleEvent* = 0);
  virtual void finalize();
  
private:
  void saveCalibrationData();
  TOFSensorTypeAssignment* m_sensorAssignment;
  unsigned int m_tofChannel;
  
};

#endif /* TOTTemperatureCorrelationPlot_hh */
