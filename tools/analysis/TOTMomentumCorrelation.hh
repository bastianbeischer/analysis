#ifndef TOTMomentumCorrelation_hh
#define TOTMomentumCorrelation_hh

#include "AnalysisPlot.hh"
#include "H2DPlot.hh"

class Hit;
class Track;
class SimpleEvent;

class TOTMomentumCorrelation : public AnalysisPlot, public H2DPlot {
public:
  TOTMomentumCorrelation(unsigned int id);
  ~TOTMomentumCorrelation();
  virtual void processEvent(const QVector<Hit*>&, Track* = 0, SimpleEvent* = 0);
  virtual void update();
private:
  unsigned int m_id;
};

#endif
