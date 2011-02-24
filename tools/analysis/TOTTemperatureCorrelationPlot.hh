#ifndef TOTTemperatureCorrelationPlot_hh
#define TOTTemperatureCorrelationPlot_hh

#include "AnalysisPlot.hh"
#include "H2DPlot.hh"
#include "SensorTypes.hh"

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
private:
  unsigned short m_id;
};

#endif /* TOTTemperatureCorrelationPlot_hh */
