#ifndef TOTTimeCorrelationPlot_hh
#define TOTTimeCorrelationPlot_hh

#include "AnalysisPlot.hh"
#include "H2DProjectionPlot.hh"
#include "SensorTypes.hh"

#include <QVector>
#include <QMap>
#include <QDateTime>

class Hit;
class Particle;
class SimpleEvent;

class TOTTimeCorrelationPlot : public AnalysisPlot, public H2DProjectionPlot {
public:
  TOTTimeCorrelationPlot(unsigned int tofChannel, QDateTime first, QDateTime last);
  ~TOTTimeCorrelationPlot();
  virtual void processEvent(const QVector<Hit*>&, const Particle* const = 0, const SimpleEvent* const = 0);
private:
  unsigned short m_id;
};

#endif /* TOTTimeCorrelationPlot_hh */
