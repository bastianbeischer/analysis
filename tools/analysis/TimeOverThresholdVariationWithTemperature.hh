#ifndef TimeOverThresholdVariationWithTemperature_hh
#define TimeOverThresholdVariationWithTemperature_hh

#include "AnalysisPlot.hh"
#include "H2DPlot.hh"
#include "SensorTypes.hh"
#include "TOFSensorTypeAssignment.hh"

#include <QVector>
#include <QMap>

class Hit;
class Track;
class SimpleEvent;

class TimeOverThresholdVariationWithTemperature : public AnalysisPlot, public H2DPlot {
public:
  TimeOverThresholdVariationWithTemperature(unsigned int tofChannel);
  ~TimeOverThresholdVariationWithTemperature();
  
  virtual void processEvent(const QVector<Hit*>&, Track* = 0, SimpleEvent* = 0);
  virtual void finalize();
  
private:
  void saveCalibrationData();
  TOFSensorTypeAssignment* m_sensorAssignment;
  unsigned int m_tofChannel;
  
};

#endif /* TimeOverThresholdVariationWithTemperature_hh */
