#ifndef TimeOverThresholdMomentumCorrelation_hh
#define TimeOverThresholdMomentumCorrelation_hh

#include "AnalysisPlot.hh"
#include "H2DPlot.hh"

class Hit;
class Track;
class SimpleEvent;

class TimeOverThresholdMomentumCorrelation : public AnalysisPlot, public H2DPlot {
public:
  TimeOverThresholdMomentumCorrelation(unsigned int id);
  ~TimeOverThresholdMomentumCorrelation();
  virtual void processEvent(const QVector<Hit*>&, Track* = 0, SimpleEvent* = 0);
  virtual void update();
private:
  unsigned int m_id;
};

#endif
