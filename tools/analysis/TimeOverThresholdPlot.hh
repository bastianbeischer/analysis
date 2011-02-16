#ifndef TimeOverThresholdPlot_hh
#define TimeOverThresholdPlot_hh

#include "AnalysisPlot.hh"
#include "H2DPlot.hh"

#include <QVector>

class Hit;
class Track;
class SimpleEvent;

class TimeOverThresholdPlot : public AnalysisPlot, public H2DPlot {
public:
  TimeOverThresholdPlot();
  ~TimeOverThresholdPlot();
  virtual void processEvent(const QVector<Hit*>&, Track* = 0, SimpleEvent* = 0);
};

#endif /* TimeOverThresholdPlot_hh */
