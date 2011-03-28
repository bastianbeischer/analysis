#ifndef SensorTimePlot_hh
#define SensorTimePlot_hh

#include "AnalysisPlot.hh"
#include "H2DPlot.hh"
#include "SensorTypes.hh"

#include <QDateTime>

class SimpleEvent;

class SensorTimePlot : public AnalysisPlot, public H2DPlot {
public:
  SensorTimePlot(SensorTypes::Type, QDateTime first, QDateTime last, int nBinsY, double min, double max);
  virtual ~SensorTimePlot();
  virtual void processEvent(const QVector<Hit*>&, Track* = 0, SimpleEvent* = 0);
protected:
  SensorTypes::Type m_type;
};

#endif
