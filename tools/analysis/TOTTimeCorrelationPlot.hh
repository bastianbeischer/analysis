#ifndef TOTTimeCorrelationPlot_hh
#define TOTTimeCorrelationPlot_hh

#include "AnalysisPlot.hh"
#include "H2DPlot.hh"
#include "SensorTypes.hh"

#include <QVector>
#include <QMap>

class Hit;
class Track;
class SimpleEvent;

class TOTTimeCorrelationPlot : public AnalysisPlot, public H2DPlot {
public:
  TOTTimeCorrelationPlot(unsigned int tofChannel);
  ~TOTTimeCorrelationPlot();
  virtual void processEvent(const QVector<Hit*>&, Track* = 0, SimpleEvent* = 0);
private:
  unsigned short m_id;
  double m_minTime;
  double m_maxTime;
  double msToMin(double timeInMs);
  double convertTime(unsigned int time);
};
#endif /* TOTTimeCorrelationPlot_hh */
