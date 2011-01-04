#ifndef TimeDifferencePlot_hh
#define TimeDifferencePlot_hh

#include "AnalysisPlot.hh"
#include "H2DPlot.hh"

class TimeDifferencePlot : public AnalysisPlot, public H2DPlot {
public:
  TimeDifferencePlot(unsigned short topBarId, unsigned short bottomBarId);
  virtual ~TimeDifferencePlot();
  virtual void processEvent(const QVector<Hit*>&, Track* = 0, SimpleEvent* = 0);
private:
  unsigned int m_topBarId;
  unsigned int m_bottomBarId;
};

#endif
